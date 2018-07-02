#include "..\include\FullClient.h"

FullClient::FullClient()
{
	_user.set_username("Oxydros");
	_tcpClient.setPacketCallback([this](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
	{
		this->handlePacket(co, packet);
	});
}


FullClient::~FullClient()
{
	if (_threadTCP)
	{
		_threadTCP->join();
		delete _threadTCP;
	}

	if (_threadUDP)
	{
		_threadUDP->join();
		delete _threadUDP;
	}
}

void FullClient::run()
{
	_tcpClient.run();
}

void FullClient::runAsync()
{
	if (_threadTCP)
		throw std::runtime_error("A thread is already running for this client instance");
	_threadTCP = new boost::thread([&] {
		_tcpClient.run();
	});
}

void FullClient::connect(std::string const &ip, std::string const &port)
{
	auto packet = std::make_shared<Network::TCPPacket>();

	TRACE("Connecting to " << ip << ":" << port << std::endl);
	_tcpClient.connect(ip, port);

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
	packet->getTCPPacket().mutable_authmessage()->set_code(0);
	packet->getTCPPacket().mutable_authmessage()->set_allocated_user(&_user);

	_tcpClient.sendPacket(packet);

	//Release allocated user to get ownership of the data back
	//This way protobuf wont delete the resource
	packet->getTCPPacket().mutable_authmessage()->release_user();
}

void FullClient::disconnect()
{
	_tcpClient.disconnect();
}

void FullClient::setUsername(std::string const &username)
{
	_user.set_username(username);
}

void FullClient::listServerFiles(std::string const &path)
{
	auto packet = std::make_shared<Network::TCPPacket>();
	std::string filePath = path;

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE);
	packet->getTCPPacket().mutable_listfilemessage()->set_allocated_pathtolist(&filePath);
	packet->getTCPPacket().mutable_listfilemessage()->set_userid(_id);

	_tcpClient.sendPacket(packet);

	packet->getTCPPacket().mutable_listfilemessage()->release_pathtolist();
}

void FullClient::listServerUsers()
{
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_LIST_USER);
	packet->getTCPPacket().mutable_listusermessage()->set_userid(_id);

	_tcpClient.sendPacket(packet);
}

void FullClient::requestUDPInfos()
{
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_UDP);
	packet->getTCPPacket().mutable_udpmessageid()->set_userid(_id);

	_tcpClient.sendPacket(packet);
}

void FullClient::entityCreateDelete(CubZPacket::EntityMessage_Action const &action, CubZPacket::EntityDescription *entityDesc)
{
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_ENTITY);

	packet->getTCPPacket().mutable_entitymessage()->set_action(CubZPacket::EntityMessage_Action(action));
	packet->getTCPPacket().mutable_entitymessage()->set_userid(GetUID());
	packet->getTCPPacket().mutable_entitymessage()->set_allocated_entity(entityDesc);

	_tcpClient.sendPacket(packet);
}

void FullClient::createEntity(CubZPacket::EntityDescription *entityDesc)
{
	TRACE("Sending entity creation Packet" << std::endl);
	entityCreateDelete(CubZPacket::EntityMessage_Action::EntityMessage_Action_ADD, entityDesc);
}

void FullClient::deleteEntity(CubZPacket::EntityDescription *entityDesc)
{
	entityCreateDelete(CubZPacket::EntityMessage_Action::EntityMessage_Action_REMOVE, entityDesc);
}

void FullClient::handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
{
	auto tcpPacket = std::static_pointer_cast<Network::TCPPacket>(packet);
	auto type = tcpPacket->getPacketType();

	TRACE("Received " << tcpPacket << std::endl);
	if (type == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
		return handleAuthPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE)
		return handleFileListPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_LIST_USER)
		return handleUserListPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_LIST_DEVICE)
		return handleDeviceListPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_UDP)
		return handleUDPPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_PING)
		return handlePingPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_ENTITY)
		return handleEntityPacket(co, tcpPacket);
}

void FullClient::handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling auth message" << std::endl);
	if (!packet->getTCPPacket().authmessage().user().username().compare(_user.username()) &&
		packet->getTCPPacket().authmessage().code() == 0 &&
		packet->getTCPPacket().authmessage().action() == CubZPacket::AuthMessage::LOGIN)
	{
		TRACE("LOGGED IN");
		_user = packet->getTCPPacket().authmessage().user();
		_id = packet->getTCPPacket().authmessage().user().id();
	}
	else if (!packet->getTCPPacket().authmessage().user().username().compare(_user.username()) &&
		packet->getTCPPacket().authmessage().code() == 0 &&
		packet->getTCPPacket().authmessage().action() == CubZPacket::AuthMessage::LOGOUT)
	{
		TRACE("LOGGED OUT");
	}
}

void FullClient::handleFileListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling file list message" << std::endl);
	auto listFilePacket = packet->getTCPPacket().listfilemessage();
	auto fileList = listFilePacket.list();
	for (auto &file : fileList)
	{
		TRACE("file found" << std::endl);
		TRACE("Got file: " << file.name() << std::endl);
	}
	TRACE("end of files" << std::endl);
}

void FullClient::handleUserListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling user list message" << std::endl);
	auto listUserPacket = packet->getTCPPacket().listusermessage();
	auto userList = listUserPacket.list();
	for (auto &user : userList)
	{
		TRACE("Got user: " << user.username() << std::endl);
	}
}

void FullClient::handleDeviceListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling device list message" << std::endl);
	auto listDevicePacket = packet->getTCPPacket().listdevicemessage();
	auto deviceList = listDevicePacket.list();
	for (auto &device : deviceList)
	{
		TRACE("Got device: " << device.name() << std::endl);
	}
}

void FullClient::handlePingPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("PING NOT IMPLEMENTED YET" << std::endl);
}

void FullClient::handleUDPPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling UDP id message" << std::endl);
	auto port = packet->getTCPPacket().udpmessageid().port();
	auto ip = packet->getTCPPacket().udpmessageid().ip();

	try {
		_udpClient.connect(ip, port);
		TRACE("UDP CONNECTED" << std::endl);
		if (_threadUDP)
			throw std::runtime_error("A thread is already running for this client instance");
		_threadUDP = new boost::thread([&] {
			_udpClient.run();
		});
	}
	catch (std::exception const &e)
	{
		TRACE("ERROR CONNECT UDP " << e.what() << std::endl);
	}
}

void FullClient::handleEntityPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	auto entity = packet->getTCPPacket().entitymessage().entity();

	auto entityAction = packet->getTCPPacket().entitymessage().action();

	TRACE("Got entity creation " << entity.id() << std::endl);
}
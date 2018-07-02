//
// Created by Louis VENTRE (aka Oxydros) on 9/13/17.
//

#ifndef LIBNETWORKS_HELLOWORLD_H
#define LIBNETWORKS_HELLOWORLD_H
#define CUBZ_NETWORK_DEBUG


#include <TCPClient.h>
#include <UDPClient.h>
#include <TCPPacket.h>
#include <UDPPacket.h>

class FullClient
{
private:
	unsigned int						_id;
	Network::TCPClient					_tcpClient{};
	Network::UDPClient					_udpClient{};
	CubZPacket::UserDescription			_user{};
	boost::thread						*_threadUDP{ nullptr };
	boost::thread						*_threadTCP{ nullptr };

public:
	FullClient();
	~FullClient();

public:
	void run();
	void runAsync();

	void connect(std::string const &ip, std::string const &port);
	void disconnect();

	void setUsername(std::string const &username);

	void listServerFiles(std::string const &path);
	void listServerUsers();

	void requestUDPInfos();

	void createEntity(CubZPacket::EntityDescription *entityDesc);
	void deleteEntity(CubZPacket::EntityDescription *entityDesc);

	unsigned int GetUID() { return _id; }

private:
	void handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet);

	void handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
	void handleFileListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
	void handleUserListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
	void handleDeviceListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
	void handlePingPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
	void handleUDPPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
	void handleEntityPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);

private:
	void entityCreateDelete(CubZPacket::EntityMessage_Action const &action, CubZPacket::EntityDescription *entityDesc);
};


#endif //LIBNETWORKS_HELLOWORLD_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

#define TEST_FILE   ("test.txt")

int main()
{
    std::ifstream infile(TEST_FILE, std::ios::in | std::ios::binary);

    std::cout << "Sending file " << TEST_FILE
              << " with size " << boost::filesystem::file_size(TEST_FILE)
              << std::endl;

    Network::ByteBuffer buffer(boost::filesystem::file_size(TEST_FILE));

    infile.read(buffer.data(), boost::filesystem::file_size(TEST_FILE));

	Network::TCPClient client{};
	Network::TCPPacket::SharedPtr packet = std::make_shared<Network::TCPPacket>();

	client.setPacketCallback([&client, &buffer](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr genericPacket)
	{
		auto packet = std::static_pointer_cast<Network::TCPPacket>(genericPacket);

		std::cout << "Received " << *packet << std::endl;
		if (packet->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_FILE)
		{
			int port = packet->getTCPPacket().filemessage().port();
			std::cout << "Sending file of size " << buffer.size() << " to 127.0.0.1:" << std::to_string(port) << std::endl;
			client.sendFile("127.0.0.1", std::to_string(port), buffer);
		}
	});
	client.connect("127.0.0.1", "4242");
	//Login packet for "Oxydros"

	CubZPacket::FileDescription *file = new CubZPacket::FileDescription();

	file->set_name("test.txt");

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_FILE);
	packet->getTCPPacket().mutable_filemessage()->set_code(0);
	packet->getTCPPacket().mutable_filemessage()->set_action(CubZPacket::FileMessage_Action::FileMessage_Action_UPLOAD);
	packet->getTCPPacket().mutable_filemessage()->set_allocated_file(file);

	client.sendPacket(packet);
	client.async_run();
	client.wait();
	TRACE("Leaving client ..." << std::endl);
    return 0;
}

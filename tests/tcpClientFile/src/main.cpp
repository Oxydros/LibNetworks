#include <iostream>
#include <boost/filesystem.hpp>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main(int argc, char **argv)
{
    assert(argc == 2);
    std::ifstream infile(argv[1], std::ios::in | std::ios::binary);

    std::cout << argv[1] << std::endl;

    std::cout << "Sending file " << argv[1]
              << " with size " << boost::filesystem::file_size(argv[1])
              << std::endl;

    char *data = new char[boost::filesystem::file_size(argv[1])];

    infile.read(data, boost::filesystem::file_size(argv[1]));

	Network::TCPClient client;
	Network::TCPPacket packet;
    CubZPacket::FileDescription *fileDescription = new CubZPacket::FileDescription();
    CubZPacket::UserDescription *user = new CubZPacket::UserDescription();

    client.setCallback([](Network::IConnection::SharedPtr co, Network::IPacket const &packet){
        auto tcpPacket = static_cast<Network::TCPPacket const &>(packet);

        std::cout << "Received " << tcpPacket << std::endl;
    });
	client.connect("127.0.0.1", "4242");

    packet.setType(Network::TCPPacket::Type::PacketTCP_Type_FILE);

    packet.getMutableFileMessage()->set_action(CubZPacket::FileMessage_Action::FileMessage_Action_UPLOAD);
    packet.getMutableFileMessage()->set_allocated_user(user);
    packet.getMutableFileMessage()->set_allocated_file(fileDescription);

    user->set_username("Oxydros");
    fileDescription->set_path(argv[1]);
    fileDescription->set_size(boost::filesystem::file_size(argv[1]));
    packet.setFileData(data, boost::filesystem::file_size(argv[1]));

    client.sendPacket(packet);
	client.run(); //Block
	dout << "Leaving client ..." << std::endl;
    return 0;
}
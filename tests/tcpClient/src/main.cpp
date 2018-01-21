#include <iostream>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main()
{
	Network::TCPClient client{};
	Network::TCPPacket packet{};
    CubZPacket::UserDescription user{};

    client.setCallback([](Network::IConnection::SharedPtr co, Network::IPacket const &packet){
        auto tcpPacket = static_cast<Network::TCPPacket const &>(packet);

        std::cout << "Received " << tcpPacket << std::endl;
        if (tcpPacket.getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
        {
            if (!tcpPacket.getAuthMessage().user().username().compare("Oxydros"))
            {
                std::cout << "Logged in" << std::endl;
            }
        }
    });
	client.connect("127.0.0.1", "4242");
    //Login packet for "Oxydros"
    packet.setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
    packet.getMutableAuthMessage()->set_code(0);
    user.set_username("Oxydros");
    packet.getMutableAuthMessage()->set_allocated_user(&user);

    client.sendPacket(packet);
	client.run(); //Block
	dout << "Leaving client ..." << std::endl;
    return 0;
}
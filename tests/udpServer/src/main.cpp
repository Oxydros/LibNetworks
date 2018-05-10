#include <iostream>
#include <UDPServer.h>
#include <UDPPacket.h>

int main()
{
	Network::UDPServer server(4242);

    server.setCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet){
        auto udpPacket = std::static_pointer_cast<Network::UDPPacket>(packet);

        std::cout << "Received " << *udpPacket << std::endl;
    });
	server.run(); //Block
    return 0;
}
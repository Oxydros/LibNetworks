#include <iostream>
#include <UDPServer.h>
#include <UDPPacket.h>

int main()
{
	Network::UDPServer server(4242);

    server.setCallback([](Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet){
        auto udpPacket = dynamic_cast<Network::UDPPacket*>(packet.get());

        std::cout << "Received " << *udpPacket << std::endl;
    });
	server.run(); //Block
    return 0;
}
#include <iostream>
#include <UDPServer.h>
#include <UDPPacket.h>

int main()
{
	Network::UDPServer server(4242);

    server.setCallback([](Network::IConnection::SharedPtr co, Network::IPacket const &packet){
        auto tcpPacket = static_cast<Network::UDPPacket const &>(packet);

        std::cout << "Received " << tcpPacket << std::endl;
    });
	server.run(); //Block
    return 0;
}
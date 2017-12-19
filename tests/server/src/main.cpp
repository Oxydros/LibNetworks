#include <iostream>
#include <Debug.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main()
{
	Network::TCPServer server("0.0.0.0", "4242");

    server.setCallback([](Network::IConnection::SharedPtr co, Network::IPacket const &packet){
        auto tcpPacket = static_cast<Network::TCPPacket const &>(packet);

        std::cout << "Received " << tcpPacket << std::endl;
    });
	server.run(); //Block
    return 0;
}
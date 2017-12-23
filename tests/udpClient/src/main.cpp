#include <iostream>
#include <Debug.h>
#include <UDPClient.h>
#include <UDPPacket.h>

int main()
{
	Network::UDPClient client;
	Network::UDPPacket packet;

    client.setCallback([](Network::IConnection::SharedPtr co, Network::IPacket const &packet){
        auto tcpPacket = static_cast<Network::UDPPacket const &>(packet);

        //std::cout << "Received " << tcpPacket << std::endl;
    });
	client.connect("127.0.0.1", "4242");

    client.sendPacket(packet);
    packet.setString("Oxydros");
    packet.setType(Network::UDPPacket::Type::PacketUDP_Type_OBJECT_MOVEMENT);
    client.sendPacket(packet);

	client.run(); //Block
	dout << "Leaving client ..." << std::endl;
    return 0;
}
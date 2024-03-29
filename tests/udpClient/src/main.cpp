#include <iostream>
#include <Debug.h>
#include <UDPClient.h>
#include <UDPPacket.h>

int main()
{
	Network::UDPClient client;
	Network::UDPPacket::SharedPtr packet = std::make_shared<Network::UDPPacket>();

    client.setPacketCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
                             {
                                 auto tcpPacket = std::static_pointer_cast<Network::UDPPacket>(packet);

                                 //std::cout << "Received " << *tcpPacket << std::endl;
                             });
	client.connect("127.0.0.1", "4242");

    client.sendPacket(packet);
    packet->setType(Network::UDPPacket::Type::PacketUDP_Type_ENTITY);
    client.sendPacket(packet);

	client.run(); //Block
	TRACE("Leaving client ..." << std::endl);
    return 0;
}
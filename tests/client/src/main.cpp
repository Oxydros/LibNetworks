#include <iostream>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

class Observer : public Network::PacketObserver
{
	virtual void handlePacket(Network::IConnection::SharedPtr from, Network::IPacket const &packet)
	{
        auto tcpPacket = static_cast<Network::TCPPacket const &>(packet);

        std::cout << "Received " << tcpPacket << std::endl;
	}
};

int main()
{
	Observer		obs;
	Network::TCPClient client(obs);
	Network::TCPPacket packet;

	client.connect("127.0.0.1", "4242");
    client.sendPacket(packet);
    packet.setType(Network::TCPPacket::Type::PacketTCP_Type_PING);
    client.sendPacket(packet);
	client.run(); //Block
	dout << "Leaving client ..." << std::endl;
    return 0;
}
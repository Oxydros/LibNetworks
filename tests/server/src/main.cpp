#include <iostream>
#include <Debug.h>
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
	Network::TCPServer server("0.0.0.0", "4242", obs);

	server.run(); //Block
    return 0;
}
#include <iostream>
#include <Debug.h>
#include <TCPServer.h>
#include <TCPPacket.h>

class Observer : public Network::PacketObserver
{
	virtual void handlePacket(Network::IConnection const &from, Network::IPacket const &packet)
	{
		std::cout << "Received new packet in handler !" << std::endl;
	}
};

int main()
{
	Observer		obs;
	Network::TCPServer server("0.0.0.0", "4242", obs);

	server.run();
    return 0;
}
#include <iostream>
#include <Debug.h>
#include <TCPServer.h>
#include <TCPPacket.h>

class Observer : public Network::PacketObserver
{
	virtual Network::IPacket *handlePacket(Network::IConnection const &from, Network::IPacket const &packet)
	{
		std::cout << "Received new packet !" << std::endl;
		return nullptr;
	}
};

int main()
{
	Observer		obs;
	Network::TCPServer server("0.0.0.0", "4242", obs);

	server.run();
    return 0;
}
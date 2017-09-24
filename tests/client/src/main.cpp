#include <iostream>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPPacket.h>

class Observer : public Network::PacketObserver
{
	virtual void handlePacket(Network::IConnection const &from, Network::IPacket const &packet)
	{
		std::cout << "Received new packet in handler!" << std::endl;
	}
};

int main()
{
	Observer		obs;
	Network::TCPClient client(obs);

	client.connect("0.0.0.0", "4242");
	client.run();
	dout << "Leaving client ..." << std::endl;
    return 0;
}
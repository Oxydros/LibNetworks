#include "UDPConnection.h"

using namespace Network;

Network::UDPConnection::UDPConnection(boost::asio::io_service &io_service)
{

}

Network::UDPConnection::~UDPConnection()
{
}

void Network::UDPConnection::start()
{
}

void Network::UDPConnection::stop()
{
}

bool Network::UDPConnection::sendPacket(IPacket const &packet)
{
	return (false);
}

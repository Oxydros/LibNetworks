#include "UDPConnection.h"

using namespace Network;

Network::UDPConnection::UDPConnection(boost::asio::io_service &io_service)
{

}

Network::UDPConnection::~UDPConnection()
{
}

bool Network::UDPConnection::start()
{
	return false;
}

bool Network::UDPConnection::stop()
{
	return false;
}

ConnectionInfo const & Network::UDPConnection::getConnectionInfo() const
{
	return (*_connectionInfo);
}

bool Network::UDPConnection::write(std::string const & data)
{
	return false;
}

std::string Network::UDPConnection::read()
{
	return std::string();
}
#include "TCPConnection.h"

using namespace Network;

TCPConnection::TCPConnection(boost::asio::io_service &io_service)
{

}

Network::TCPConnection::~TCPConnection()
{
}

bool Network::TCPConnection::start()
{
	return false;
}

bool Network::TCPConnection::stop()
{
	return false;
}

ConnectionInfo const & Network::TCPConnection::getConnectionInfo() const
{
	return (*_connectionInfo);
}

bool Network::TCPConnection::write(std::string const & data)
{
	return false;
}

std::string Network::TCPConnection::read()
{
	return std::string();
}
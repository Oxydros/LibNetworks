#include "TCPConnection.h"

using namespace Network;

TCPConnection::TCPConnection(boost::asio::ip::tcp::socket socket,
	ConnectionManager &manager, PacketObserver &observer)
	: _socket(std::move(socket)), _connectionManager(manager), _observer(observer)
{

}

Network::TCPConnection::~TCPConnection()
{
}

bool Network::TCPConnection::start()
{
	process_read();
	return false;
}

bool Network::TCPConnection::stop()
{
	_socket.close();
	return false;
}

ConnectionInfo const & Network::TCPConnection::getConnectionInfo() const
{
	return (*_connectionInfo);
}

void Network::TCPConnection::process_read()
{
	//We use this to make sure that the connection object is still alive
	//during the async operations
	auto	selfPtr = shared_from_this();

	//We read 4 bytes to determine the size of the packet.
	//Then we will read of the size of the packet
	_socket.async_read_some(boost::asio::buffer(&(_buffer[0]), 4),
		[this, selfPtr](boost::system::error_code errorCode, std::size_t nbBytes)
	{
		//Here _buffer is filled with nbBytes
		//We can do stuff to read the packet etc.
		//We can then call again process_read
		//We can also call the observer and write the response with a 
		//process write
	});
}

void Network::TCPConnection::process_write()
{
}
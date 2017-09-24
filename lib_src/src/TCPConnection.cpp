#include "TCPConnection.h"

using namespace Network;

TCPConnection::TCPConnection(boost::asio::ip::tcp::socket socket,
	PacketObserver &observer, ConnectionManager *manager)
	: _socket(std::move(socket)), _connectionManager(manager), _observer(observer),
      _stopped(false)
{
    boost::asio::ip::tcp::socket::non_blocking_io non_blocking_io(true);
    _socket.io_control(non_blocking_io);
    _socket.non_blocking(true);
}

void Network::TCPConnection::start()
{
    if (_stopped)
        return ;
	checkRead();
	if (!_toSendBuffer.empty() && !_isWriting)
		checkWrite();
}

void Network::TCPConnection::checkRead()
{
	dout << "Checking if I can read" << std::endl;
	//Check if we can read on the socket
	//select() like
	_socket.async_read_some(boost::asio::null_buffers(),
							boost::bind(&TCPConnection::handleRead,
										shared_from_this(),
										boost::asio::placeholders::error));
}

void Network::TCPConnection::checkWrite()
{
	dout << "Checking if I can write" << std::endl;
	//Check if we can write on the socket
	//select() like
	_isWriting = true;
	_socket.async_write_some(boost::asio::null_buffers(),
							 boost::bind(&TCPConnection::handleWrite,
										 shared_from_this(),
										 boost::asio::placeholders::error));
}

void Network::TCPConnection::stop()
{
    if (!_stopped)
    {
        dout << "Stop socket" << std::endl;
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        _socket.close();
        _stopped = true;
    }
}

bool Network::TCPConnection::sendPacket(IPacket const &packet)
{
	PacketBuffer toSend = packet.getData();

	//Concat buffers
	_toSendBuffer.insert(std::end(_toSendBuffer), std::begin(toSend),
	std::end(toSend));
	return (false);
}

void Network::TCPConnection::handleRead(boost::system::error_code ec)
{
    dout << ec.message() << std::endl;
	if (!ec)
	{
		processRead(ec);
	}
	if (!ec || ec == boost::asio::error::would_block)
		start();
	else
		_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

void Network::TCPConnection::handleWrite(boost::system::error_code ec)
{
	_isWriting = false;
    dout << ec.message() << std::endl;
	if (!ec)
	{
		processWrite(ec);
	}
	//If error, stop socket
	if (!(!ec || ec == boost::asio::error::would_block))
		_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

void Network::TCPConnection::processRead(boost::system::error_code &ec)
{
    std::size_t len;

    len = _socket.read_some(boost::asio::buffer(_buffer, 512), ec);
    dout << "READ " << len << std::endl;
    if (len == 0)
        _connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

void Network::TCPConnection::processWrite(boost::system::error_code &ec)
{
    dout << "WRITING on SOCKET" << std::endl;
}
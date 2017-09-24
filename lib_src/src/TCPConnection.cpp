#include "TCPConnection.h"

using namespace Network;

TCPConnection::TCPConnection(boost::asio::ip::tcp::socket socket,
	PacketObserver &observer, ConnectionManager *manager)
	: _socket(std::move(socket)), _connectionManager(manager), _observer(observer),
      _stopped(false), _buffer()
{
    _buffer.reserve(512);
}

void Network::TCPConnection::start()
{
	processRead();
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

void Network::TCPConnection::processRead()
{
	auto self(shared_from_this());

	dout << "Launch async read" << std::endl;
	_socket.async_read_some(boost::asio::buffer(&_buffer[0], 4),
	[this, self](boost::system::error_code ec, std::size_t nbBytes)
	{
		dout << "READ " << nbBytes << std::endl;
		if (!ec && nbBytes > 0)
        {
            processRead();
        }
		else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
		{
            dout << "Read error, stopping socket" << std::endl;
			_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
		}
	});
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

	dout << "Received send packet cmd" << std::endl;
	//Concat buffers
	_toSendBuffer.insert(std::end(_toSendBuffer), std::begin(toSend),
	std::end(toSend));
	checkWrite();
	return (false);
}

void Network::TCPConnection::handleWrite(boost::system::error_code ec)
{
	_isWriting = false;
    dout << "WRITE: " << ec.message() << std::endl;
	if (!ec)
	{
		processWrite(ec);
	}
	//If error, stop socket
	if (!(!ec || ec == boost::asio::error::would_block))
		_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

void Network::TCPConnection::processWrite(boost::system::error_code &ec)
{
    dout << "WRITING on SOCKET" << std::endl;
}
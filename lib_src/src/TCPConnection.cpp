#include <TCPPacket.h>
#include "TCPConnection.h"

using namespace Network;

TCPConnection::TCPConnection(boost::asio::strand &strand, boost::asio::ip::tcp::socket socket,
	PacketObserver &observer, ConnectionManager *manager)
	: _strand(strand), _socket(std::move(socket)), _connectionManager(manager), _callBack(observer),
      _stopped(false), _buffer(), _bytesSize(0), _ioMutex(), _packetSize(0), _packetRead(0)
{
}

void Network::TCPConnection::start()
{
	processRead();
}

void Network::TCPConnection::processRead()
{
	auto                            self(shared_from_this());

	dout << "Launch async read for packet size. sizeof(int32_t)=" << sizeof(int32_t) << std::endl;
	_socket.async_read_some(boost::asio::buffer(&_bytesSize, 4),
	[this, self](boost::system::error_code ec, std::size_t nbBytes)
	{
        std::int32_t                         bytesToRead = 0;

        dout << "READ " << nbBytes << std::endl;
        std::memcpy(&bytesToRead, &_bytesSize, sizeof(bytesToRead));
        dout << "Packet size to receive is " << bytesToRead << std::endl;


		if (!ec && nbBytes > 0)
        {
            _packetSize = bytesToRead;
            _packetRead = 0;

            dout << "Launch async read for packet" << std::endl;
            _buffer.resize(bytesToRead);

            handleRead(bytesToRead);
        }
		else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
		{
            dout << "Read error, stopping socket" << std::endl;
			_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
		}
	});
}

void Network::TCPConnection::handleRead(size_t bytesToRead)
{
    auto self(shared_from_this());

    dout << "Handle read called with " << bytesToRead << std::endl;
    _socket.async_read_some(boost::asio::buffer(_buffer.data(), bytesToRead),
                            [this, self](boost::system::error_code ec, std::size_t nbBytes)
                            {
                                _packetRead += nbBytes;
                                dout << "READ " << nbBytes << ". State: " << _packetRead << "/" << _packetSize <<std::endl;
                                if (!ec && nbBytes > 0)
                                {
                                    if (_packetRead != _packetSize)
                                        handleRead(_packetSize - _packetRead);
                                    else
                                    {
                                        TCPPacket packet;

                                        packet.setData(_buffer);
                                        _callBack(self, packet);
                                        processRead();
                                    }
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
    boost::mutex::scoped_lock   lock(_ioMutex);
    if (!_stopped)
    {
        dout << "Stop socket" << std::endl;
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        _socket.close();
        _stopped = true;
    }
}

//Scoped lock to prevent multiple thread write bugs
bool Network::TCPConnection::sendPacket(IPacket const &packet)
{
    boost::mutex::scoped_lock   lock(_ioMutex);
	PacketBuffer toSend = packet.getData();

	dout << "Received send packet cmd of size " << toSend.size() << std::endl;
	//Concat buffers
	_toSendBuffer.insert(std::end(_toSendBuffer), std::begin(toSend),
	std::end(toSend));
	checkWrite();
	return (false);
}

void Network::TCPConnection::handleWrite(boost::system::error_code ec)
{
    dout << "WRITE: " << ec.message() << std::endl;
	if (!ec)
	{
		processWrite(ec);
	}
	//If error, stop socket
	if (!(!ec || ec == boost::asio::error::would_block))
		_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

//Scoped lock to prevent multiple thread write bugs
void Network::TCPConnection::processWrite(boost::system::error_code &ec)
{
    boost::mutex::scoped_lock   lock(_ioMutex);

    dout << "WRITING on SOCKET " << _toSendBuffer.size() << " bytes" << std::endl;
    std::size_t len = _socket.write_some(boost::asio::buffer(_toSendBuffer.data(), _toSendBuffer.size()), ec);
    _toSendBuffer.clear();
    dout << "SUCCESSFULLY WROTE " << len << std::endl;
}

void Network::TCPConnection::checkWrite()
{
    dout << "Checking if I can write" << std::endl;
    _socket.async_write_some(boost::asio::null_buffers(),
                             _strand.wrap(
                             boost::bind(&TCPConnection::handleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error)));
}

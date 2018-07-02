#include <TCPPacket.h>
#include "TCPPacketConnection.h"
#include "PacketParser.h"

using namespace Network;

TCPPacketConnection::TCPPacketConnection(boost::asio::io_service &io_service, boost::asio::ip::tcp::socket socket,
	PacketCallback &observer, TCPConnectionManager *manager)
	: _strand(io_service), _socket(std::move(socket)), _connectionManager(manager), _callBack(observer),
      _stopped(false), _ioMutex(), _readBuffer(MAX_BUFFER_SIZE), _readActionBuffer(),
      _toSendBuffer(MAX_BUFFER_SIZE)
{
    _readActionBuffer.resize(READ_SIZE);
}

void Network::TCPPacketConnection::start()
{
	processRead();
}

void Network::TCPPacketConnection::processRead()
{
	auto                            self{shared_from_this()};

	TCPMSG("Launch async read for packet" << std::endl);
	_socket.async_read_some(boost::asio::buffer(_readActionBuffer.data(), READ_SIZE),
	_strand.wrap([this, self](boost::system::error_code ec, std::size_t nbBytes)
	{

		if (!ec && nbBytes > 0)
        {
			TCPMSG("Read " << nbBytes << std::endl);
            assert(_readBuffer.reserve() >= nbBytes);

            _readBuffer.insert(_readBuffer.end(), _readActionBuffer.begin(), _readActionBuffer.begin() + nbBytes);
            _readActionBuffer.clear();
			TCPMSG("Circular buffer size is now: " << _readBuffer.size() << std::endl);
            while (true)
            {
                auto packet = Network::extractPacketFromCircularBuffer<TCPPacket>(_readBuffer);
                if (packet)
                {
                    _callBack(shared_from_this(), packet);
                    if (_readBuffer.empty())
                        break;
                } else {
					TCPMSG("No more packet readble, size of circular is " << _readBuffer.size() << std::endl);
                    break;
                }
            }
            start();
        }
		else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
		{
			TCPMSG("Read error, stopping socket" << std::endl);
			_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
		}
	}));
}

void Network::TCPPacketConnection::stop()
{
    boost::mutex::scoped_lock   lock{_ioMutex};
    if (!_stopped)
    {
		TCPMSG("Stop socket" << std::endl);
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        _socket.close();
        _stopped = true;
    }
}

//Scoped lock to prevent multiple thread write bugs
bool Network::TCPPacketConnection::sendPacket(IPacket::SharedPtr packet)
{
    boost::mutex::scoped_lock   lock{_ioMutex};
    ByteBuffer	            _finalBuffer{};
    ByteBuffer                toSend{packet->getData()};
    PacketSize                  packetSize{static_cast<int>(toSend.size())};

    if (_toSendBuffer.reserve() < packetSize)
    {
		TCPMSG("OVERFLOW !!" << std::endl);
        assert(false);
    }
	TCPMSG("Received send packet cmd of size " << toSend.size() << std::endl);
    _finalBuffer.resize(sizeof(PacketSize));
    std::memcpy(_finalBuffer.data(), &packetSize, sizeof(packetSize));
    _toSendBuffer.insert(_toSendBuffer.end(), _finalBuffer.begin(), _finalBuffer.end());
    _toSendBuffer.insert(_toSendBuffer.end(), toSend.begin(), toSend.end());
	TCPMSG("Send buffer size is now " << _toSendBuffer.size() << std::endl);
    //Check if we can write some data in the socket
	checkWrite();
	return (false);
}

void Network::TCPPacketConnection::checkWrite()
{
	TCPMSG("Checking if I can write" << std::endl);
    _socket.async_write_some(boost::asio::null_buffers(),
                             _strand.wrap(
                                     boost::bind(&TCPPacketConnection::handleWrite,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error)));
}

void Network::TCPPacketConnection::handleWrite(boost::system::error_code ec)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    if (_toSendBuffer.empty())
        return;
	TCPMSG("Write: " << ec.message() << std::endl);
	if (!ec)
    {
		TCPMSG("Writing on socket " << _toSendBuffer.size() << " bytes" << std::endl);
        std::size_t len = _socket.write_some(boost::asio::buffer(_toSendBuffer.linearize(), _toSendBuffer.size()), ec);
		TCPMSG("Successfully wrote " << len << std::endl);
        _toSendBuffer.erase_begin(len);
		TCPMSG("New size of send buffer " << _toSendBuffer.size() << std::endl);
        if (!_toSendBuffer.empty())
            checkWrite();
    }
	//If error, stop socket
	if (!(!ec || ec == boost::asio::error::would_block))
		_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

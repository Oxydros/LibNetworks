#include <TCPPacket.h>
#include "TCPRawConnection.h"
#include "PacketParser.h"

using namespace Network;

TCPRawConnection::TCPRawConnection(boost::asio::io_service &io_service, boost::asio::ip::tcp::socket socket,
	RawEndCallback observer, TCPConnectionManager *manager)
	: _strand(io_service), _socket(std::move(socket)), _connectionManager(manager), _callBack(observer),
      _stopped(false), _ioMutex(), _readBuffer(), _readActionBuffer(),
      _toSendBuffer()
{
    _readActionBuffer.resize(READ_SIZE);
}

void Network::TCPRawConnection::start()
{}

void Network::TCPRawConnection::processRead()
{
	auto                            self{shared_from_this()};

	TCPMSG("Launch async read for file data" << std::endl);
	_socket.async_read_some(boost::asio::buffer(_readActionBuffer.data(), READ_SIZE),
	_strand.wrap([this, self](boost::system::error_code ec, std::size_t nbBytes)
	{
        boost::mutex::scoped_lock   lock{_ioMutex};

		if (!ec && nbBytes > 0)
        {
			TCPMSG("Read " << nbBytes << std::endl);

            _readBuffer.insert(_readBuffer.end(), _readActionBuffer.begin(), _readActionBuffer.begin() + nbBytes);
            _readActionBuffer.clear();
			TCPMSG("Read buffer size is now: " << _readBuffer.size() << std::endl);
			TCPMSG("Expected read is " << _expectReadSize << std::endl);
            if (_expectRead && _expectReadSize == _readBuffer.size())
            {
                _callBack(shared_from_this(), _readBuffer);
			}
			else
			{
				processRead();
			}
        }
		else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
		{
			TCPMSG("Read error, stopping socket" << std::endl);
			_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
		}
	}));
}

void Network::TCPRawConnection::stop()
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

void TCPRawConnection::readRawBytes(size_t expectedSize)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    if (_expectRead)
        throw std::runtime_error("Can't have to expect read on the same RAW Connection");
    _expectRead = true;
    _expectReadSize = expectedSize;
    processRead();
}

//Scoped lock to prevent multiple thread write bugs
bool Network::TCPRawConnection::sendRawBytes(ByteBuffer bytes)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

	TCPMSG("Received send file data of size " << bytes.size() << std::endl);
    _toSendBuffer.insert(_toSendBuffer.end(), bytes.begin(), bytes.begin() + bytes.size());
	TCPMSG("Send buffer size is now " << _toSendBuffer.size() << std::endl);
    //Check if we can write some data in the socket
	checkWrite();
	return (false);
}

void Network::TCPRawConnection::checkWrite()
{
	TCPMSG("Checking if I can write" << std::endl);
    _socket.async_write_some(boost::asio::null_buffers(),
                             _strand.wrap(
                                     boost::bind(&TCPRawConnection::handleWrite,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error)));
}

void Network::TCPRawConnection::handleWrite(boost::system::error_code ec)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    if (_toSendBuffer.empty())
        return;
	TCPMSG("Write: " << ec.message() << std::endl);
	if (!ec)
    {
		TCPMSG("Writing on socket " << _toSendBuffer.size() << " bytes" << std::endl);
        std::size_t len = _socket.write_some(boost::asio::buffer(_toSendBuffer, _toSendBuffer.size()), ec);
		TCPMSG("Successfully wrote " << len << std::endl);
        _toSendBuffer.erase(_toSendBuffer.begin(), _toSendBuffer.begin() + len);
		TCPMSG("New size of send buffer " << _toSendBuffer.size() << " " << _toSendBuffer.empty() << std::endl);
        if (_toSendBuffer.size())
            checkWrite();
        else
            _callBack(shared_from_this(), {});
    }
	//If error, stop socket
	if (!(!ec || ec == boost::asio::error::would_block))
	{
		TCPMSG("Write error, stopping socket" << std::endl);
		_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
	}
}

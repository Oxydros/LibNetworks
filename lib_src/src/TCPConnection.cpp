#include <TCPPacket.h>
#include "TCPConnection.h"

using namespace Network;

TCPConnection::TCPConnection(boost::asio::strand &strand, boost::asio::ip::tcp::socket socket,
	PacketObserver &observer, TCPConnectionManager *manager)
	: _strand(strand), _socket(std::move(socket)), _connectionManager(manager), _callBack(observer),
      _stopped(false), _buffer(), _finalBuffer(), _bytesSize(0), _ioMutex(), _packetSize(0), _packetRead(0)
{
    _buffer.resize(BUFFER_SIZE);
}

void Network::TCPConnection::start()
{
	processRead();
}

void Network::TCPConnection::processRead()
{
	auto                            self(shared_from_this());

	dout << "TCP: Launch async read for packet size." << std::endl;
	_socket.async_read_some(boost::asio::buffer(&_bytesSize, sizeof(_bytesSize)),
	_strand.wrap([this, self](boost::system::error_code ec, std::size_t nbBytes)
	{
        std::int32_t                         bytesToRead = 0;

        dout << "TCP: READ " << nbBytes << std::endl;
        std::memcpy(&bytesToRead, &_bytesSize, sizeof(bytesToRead));
        dout << "TCP: Packet size to receive is " << bytesToRead << std::endl;


		if (!ec && nbBytes > 0)
        {
            _packetSize = bytesToRead;
            _packetRead = 0;

            dout << "TCP: Launch async read for packet" << std::endl;
            handleRead(BUFFER_SIZE);
        }
		else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
		{
            dout << "TCP: Read error, stopping socket" << std::endl;
			_connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
		}
	}));
}

void Network::TCPConnection::handleRead(size_t bytesToRead)
{
    auto self(shared_from_this());

    dout << "TCP: Handle read called with " << bytesToRead << std::endl;
    _socket.async_read_some(boost::asio::buffer(_buffer.data(), bytesToRead),
                            _strand.wrap([this, self](boost::system::error_code ec, std::size_t nbBytes)
                            {
                                _packetRead += nbBytes;
                                dout << "TCP: READ " << nbBytes << ". State: " << _packetRead << "/" << _packetSize <<std::endl;
                                if (!ec && nbBytes > 0)
                                {
                                    //Append the new bytes read to the final packet buffer
                                    _finalBuffer.insert(_finalBuffer.end(), _buffer.begin(), _buffer.end());
                                    if (_packetRead != _packetSize)
                                        handleRead(BUFFER_SIZE);
                                    else
                                    {
                                        TCPPacket packet;

                                        packet.setData(_finalBuffer);
                                        _callBack(self, packet);
                                        //Clear final buffer
                                        _finalBuffer.clear();
                                        processRead();
                                    }
                                }
                                else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
                                {
                                    _finalBuffer.clear();
                                    dout << "TCP: Read error, stopping socket" << std::endl;
                                    _connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
                                }
                            }));
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
	PacketBuffer                toSend = packet.getData();
    PacketSize                  packetSize = toSend.size();
    PacketSize                  finalPacketSize = sizeof(finalPacketSize) + packetSize;

	dout << "TCP: Received send packet cmd of size " << toSend.size() << std::endl;
	//Concat buffers
    //Receive send buffer to add new packet to send
    _toSendBuffer.resize(_toSendBuffer.size() + finalPacketSize);
    //Write the size of the new packet
    std::memcpy(_toSendBuffer.data(), &packetSize, sizeof(packetSize));
    //Write the new packet
    std::memcpy(_toSendBuffer.data() + sizeof(packetSize), toSend.data(), toSend.size());
    //Check if we can write some data in the socket
	checkWrite();
	return (false);
}

void Network::TCPConnection::handleWrite(boost::system::error_code ec)
{
    dout << "TCP: WRITE: " << ec.message() << std::endl;
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

    dout << "TCP: WRITING on SOCKET " << _toSendBuffer.size() << " bytes" << std::endl;
    std::size_t len = _socket.write_some(boost::asio::buffer(_toSendBuffer.data(), _toSendBuffer.size()), ec);
    dout << "TCP: SUCCESSFULLY WROTE " << len << std::endl;
    _toSendBuffer.erase(_toSendBuffer.begin(), _toSendBuffer.begin() + len);
    dout << "TCP: NEW SIZE OF SEND BUFFER IS " << _toSendBuffer.size() << std::endl;
    if (_toSendBuffer.size() > 0)
    {
        checkWrite();
    }
}

void Network::TCPConnection::checkWrite()
{
    dout << "TCP: Checking if I can write" << std::endl;
    _socket.async_write_some(boost::asio::null_buffers(),
                             _strand.wrap(
                             boost::bind(&TCPConnection::handleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error)));
}

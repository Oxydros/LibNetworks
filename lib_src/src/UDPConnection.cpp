#include "UDPConnectionManager.h"
#include "UDPConnection.h"

using namespace Network;

Network::UDPConnection::UDPConnection(boost::asio::io_service::strand &strand, boost::asio::ip::udp::socket &socket,
                                      Network::UDPConnection::endpoint &remote,
                                      UDPConnectionManager *manager)
        : _strand(strand), _socket(socket), _connectionManager(manager),
          _remoteEndpoint(std::move(remote)), _ioMutex(), _toSendBuffer(MAX_BUFFER_SIZE)
{

}

Network::UDPConnection::~UDPConnection()
{
}

void Network::UDPConnection::start()
{
    //EMPTY
}

void Network::UDPConnection::stop()
{
    //EMPTY
}

bool Network::UDPConnection::sendPacket(IPacket::SharedPtr packet)
{
    boost::mutex::scoped_lock   lock{_ioMutex};
    ByteBuffer	                _finalBuffer{};
    ByteBuffer                   toSend{packet->getData()};
    PacketSize                  packetSize{static_cast<int>(toSend.size())};

    if (_toSendBuffer.reserve() < packetSize)
    {
		UDPMSG("OVERFLOW !!" << std::endl);
        assert(false);
    }
	UDPMSG("Received send packet cmd of size " << toSend.size()
           << " for " << _remoteEndpoint << std::endl);
    _finalBuffer.resize(sizeof(PacketSize));
    std::memcpy(_finalBuffer.data(), &packetSize, sizeof(packetSize));
    _toSendBuffer.insert(_toSendBuffer.end(), _finalBuffer.begin(), _finalBuffer.end());
    _toSendBuffer.insert(_toSendBuffer.end(), toSend.begin(), toSend.end());
	UDPMSG("Send buffer size is now " << _toSendBuffer.size() << std::endl);
    //Check if we can write some data in the socket
    checkWrite();
    return (false);
}

void Network::UDPConnection::checkWrite()
{
	UDPMSG("UDP Checking if I can write" << std::endl);
    _socket.async_send_to(boost::asio::null_buffers(), _remoteEndpoint,
                            
                                     boost::bind(&UDPConnection::handleWrite,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error));
}

void Network::UDPConnection::handleWrite(boost::system::error_code ec)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    if (_toSendBuffer.empty())
        return;
	UDPMSG("Write: " << ec.message() << std::endl);
    if (!ec)
    {
		UDPMSG("Writing on socket " << _toSendBuffer.size() << " bytes" << std::endl);
        std::size_t len = _socket.send_to(boost::asio::buffer(_toSendBuffer.linearize(), _toSendBuffer.size()), _remoteEndpoint);
		UDPMSG("Successfully wrote " << len << std::endl);
        _toSendBuffer.erase_begin(len);
		UDPMSG("New size of send buffer " << _toSendBuffer.size() << std::endl);
        if (!_toSendBuffer.empty())
            checkWrite();
    }
    //If error, stop socket
    if (!(!ec || ec == boost::asio::error::would_block))
        _connectionManager != nullptr ? _connectionManager->stop(shared_from_this()) : stop();
}

Network::UDPConnection::endpoint const& Network::UDPConnection::getEndpoint() const noexcept
{
    return (_remoteEndpoint);
}

#include "UDPConnectionManager.h"
#include "UDPConnection.h"

using namespace Network;

Network::UDPConnection::UDPConnection(boost::asio::strand &strand, boost::asio::ip::udp::socket &socket,
                                      Network::UDPConnection::endpoint &remote,
                                      UDPConnectionManager *manager)
        : _strand(strand), _socket(socket), _connectionManager(manager),
          _remoteEndpoint(std::move(remote)), _ioMutex(), _toSendBuffer()
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

bool Network::UDPConnection::sendPacket(IPacket const &packet)
{
    boost::mutex::scoped_lock   lock(_ioMutex);
    PacketBuffer                toSend = packet.getData();
    PacketSize                  packetSize = toSend.size();
    PacketSize                  finalPacketSize = sizeof(finalPacketSize) + packetSize;

    dout << "Received send packet cmd of size  " << toSend.size()
         << " for : " << _remoteEndpoint
         << std::endl;
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

void Network::UDPConnection::checkWrite()
{
    dout << "Checking if I can write" << std::endl;
    _socket.async_send_to(boost::asio::null_buffers(), _remoteEndpoint,
                             _strand.wrap(
                                     boost::bind(&UDPConnection::handleWrite,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error)));
}

void Network::UDPConnection::handleWrite(boost::system::error_code ec)
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

void Network::UDPConnection::processWrite(boost::system::error_code &ec)
{
    boost::mutex::scoped_lock   lock(_ioMutex);

    dout << "WRITING on SOCKET " << _toSendBuffer.size() << " bytes" << std::endl;
    std::size_t len = _socket.send_to(boost::asio::buffer(_toSendBuffer.data(), _toSendBuffer.size()), _remoteEndpoint);
    dout << "SUCCESSFULLY WROTE " << len << std::endl;
    _toSendBuffer.erase(_toSendBuffer.begin(), _toSendBuffer.begin() + len);
    dout << "NEW SIZE OF SEND BUFFER IS " << _toSendBuffer.size() << std::endl;
    if (_toSendBuffer.size() > 0)
    {
        checkWrite();
    }
}

Network::UDPConnection::endpoint const& Network::UDPConnection::getEndpoint() const noexcept
{
    return (_remoteEndpoint);
}
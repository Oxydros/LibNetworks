
#include "PacketParser.h"
#include <UDPPacket.h>
#include <UDPConnectionManager.h>

using namespace Network;

Network::UDPConnectionManager::UDPConnectionManager(boost::asio::ip::udp::socket &socket,
                                                    boost::asio::io_service::strand &strand,
                                                    PacketObserver &callBack)
        : _socket(socket), _strand(strand), _callBack(callBack),
          _ioMutex(), _readBuffer(MAX_BUFFER_SIZE), _readActionBuffer(READ_SIZE)
{
}

Network::UDPConnectionManager::~UDPConnectionManager()
{
}

void Network::UDPConnectionManager::run()
{
    udpMsg << "Launch async read for packet" << std::endl;
    _socket.async_receive_from(boost::asio::buffer(_readActionBuffer.data(), READ_SIZE), _currentRemoteEndpoint,
                              _strand.wrap(
                                      [this](boost::system::error_code ec, std::size_t nbBytes)
                                      {
                                          if (!ec && nbBytes > 0)
                                          {
                                              udpMsg << "Got packet of size: " << nbBytes
                                                   << " from " << _currentRemoteEndpoint << std::endl;
                                              assert(_readBuffer.reserve() >= nbBytes);

                                              _readBuffer.insert(_readBuffer.end(), _readActionBuffer.begin(), _readActionBuffer.begin() + nbBytes);
                                              _readActionBuffer.clear();
                                              udpMsg << "Circular buffer size is now: " << _readBuffer.size() << std::endl;
                                              processRead();
                                          }
                                          else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
                                          {
                                              udpMsg << "Read error for " << _currentRemoteEndpoint << std::endl;
                                          }
                                      }
                              ));
}

void Network::UDPConnectionManager::processRead()
{
    IPacketConnection::SharedPtr remoteCo = extractRemotePacketCo(_currentRemoteEndpoint);

    assert(remoteCo);
    while (true)
    {
        auto packet = Network::extractPacketFromCircularBuffer<UDPPacket>(_readBuffer);
        if (packet)
        {
            if (_callBack)
                _callBack(remoteCo, packet);
            if (_readBuffer.empty())
                break;
        } else {
            tcpMsg << "No more packet readble, size of circular is " << _readBuffer.size() << std::endl;
            break;
        }
    }
    run();
}

void Network::UDPConnectionManager::add(IConnection::SharedPtr ptr)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    udpMsg << "New connection" << std::endl;
	//Adding the connection to the set
	_connections.insert(ptr);

	//Launching the connection read / write loop
	ptr->start();
    udpMsg << _connections.size() << " active connections" << std::endl;
}

void Network::UDPConnectionManager::stop(IConnection::SharedPtr ptr)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    udpMsg << "Delete connection" << std::endl;

	//Erasing the connection from the set
	_connections.erase(ptr);

	//Stopping the read / write loop by closing the socket
	ptr->stop();
    udpMsg << _connections.size() << " active connections" << std::endl;
}

//Stop all connections
void Network::UDPConnectionManager::stop_all()
{
    boost::mutex::scoped_lock   lock{_ioMutex};

	for (auto co : _connections)
	{
		co->stop();
	}
	_connections.clear();
}

IPacketConnection::SharedPtr Network::UDPConnectionManager::extractRemotePacketCo(boost::asio::ip::udp::endpoint &remoteEndpoint)
{
    IPacketConnection::SharedPtr co{};

    auto foundRemoteConnection = std::find_if(_connections.begin(),
                                              _connections.end(),
                                              [&](IConnection::SharedPtr ptr) -> bool
                                              {
                                                  return (remoteEndpoint == std::dynamic_pointer_cast<UDPConnection>(ptr)->getEndpoint());
                                              });
    udpMsg << "Identifying remote connection..." << std::endl;
    if (foundRemoteConnection == _connections.end())
    {
        udpMsg << "Couldn't find remote... creating a udp connection" << std::endl;
        co = std::make_shared<UDPConnection>(_strand,
                                             _socket,
                                             remoteEndpoint,
                                             this);
        add(co);
    } else {
        co = std::static_pointer_cast<IPacketConnection>(*foundRemoteConnection);
    }
    return (co);
}
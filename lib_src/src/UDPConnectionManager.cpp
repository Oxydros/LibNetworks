
#include <UDPPacket.h>
#include <UDPConnectionManager.h>

using namespace Network;

Network::UDPConnectionManager::UDPConnectionManager(boost::asio::ip::udp::socket &socket,
                                                    boost::asio::strand &strand,
                                                    PacketObserver &callBack)
        : _socket(socket), _strand(strand), _callBack(callBack), _ioMutex(), _receptBuffer(READ_SIZE)
{
}

Network::UDPConnectionManager::~UDPConnectionManager()
{
}

void Network::UDPConnectionManager::run()
{
    udpMsg << "Launch async read for packet size." << std::endl;
    _socket.async_receive_from(boost::asio::buffer(_receptBuffer.data(), READ_SIZE), _currentRemoteEndpoint,
                              _strand.wrap(
                                      [this](boost::system::error_code ec, std::size_t nbBytes)
                                      {
                                          if (!ec && nbBytes > 0)
                                          {
                                              udpMsg << "Got packet of size: " << nbBytes
                                                   << " from " << _currentRemoteEndpoint << std::endl;
                                              processRead(nbBytes);
                                          }
                                          else if (nbBytes <= 0 || ec != boost::asio::error::operation_aborted)
                                          {
                                              udpMsg << "Read error for " << _currentRemoteEndpoint << std::endl;
                                          }
                                      }
                              ));
}

void Network::UDPConnectionManager::processRead(size_t nbBytes)
{
    auto found = std::find_if(_connections.begin(), _connections.end(),
    [&](IConnection::SharedPtr ptr) -> bool
    {
        return (_currentRemoteEndpoint == std::dynamic_pointer_cast<UDPConnection>(ptr)->getEndpoint());
    });
    UDPPacket   packet{};

    packet.setData(_receptBuffer);
    if (found == _connections.end())
    {
        udpMsg << "Couldn't find remote... creating a udp connection" << std::endl;
        UDPConnection::SharedPtr newCo = std::make_shared<UDPConnection>(_strand,
                                                                         _socket,
                                                                         _currentRemoteEndpoint,
                                                                         this);
        add(newCo);
        if (_callBack)
            _callBack(newCo, packet);
    } else if (_callBack){
        _callBack(*found, packet);
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
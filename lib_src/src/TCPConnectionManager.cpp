
#include "Debug.h"
#include "TCPConnectionManager.h"

using namespace Network;

Network::TCPConnectionManager::TCPConnectionManager()
        : _ioMutex()
{
}

Network::TCPConnectionManager::~TCPConnectionManager()
{
}

void Network::TCPConnectionManager::add(IConnection::SharedPtr ptr)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

	tcpMsg << "New connection" << std::endl;
	//Adding the connection to the set
	_connections.insert(ptr);

	//Launching the connection read / write loop
	ptr->start();
    tcpMsg << _connections.size() << " active connections" << std::endl;
}

void Network::TCPConnectionManager::stop(IConnection::SharedPtr ptr)
{
    boost::mutex::scoped_lock   lock{_ioMutex};

    tcpMsg << "Delete connection" << std::endl;

	//Erasing the connection from the set
	_connections.erase(ptr);

	//Stopping the read / write loop by closing the socket
	ptr->stop();
    tcpMsg << _connections.size() << " active connections" << std::endl;
}

//Stop all connections
void Network::TCPConnectionManager::stop_all()
{
    boost::mutex::scoped_lock   lock{_ioMutex};

	for (auto co : _connections)
	{
		co->stop();
	}
	_connections.clear();
}
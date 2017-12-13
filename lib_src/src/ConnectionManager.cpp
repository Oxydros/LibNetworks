
#include "Debug.h"
#include "ConnectionManager.h"

using namespace Network;

Network::ConnectionManager::ConnectionManager()
        : _ioMutex()
{
}

Network::ConnectionManager::~ConnectionManager()
{
}

void Network::ConnectionManager::add(IConnection::SharedPtr ptr)
{
    boost::mutex::scoped_lock   lock(_ioMutex);

	dout << "New connection" << std::endl;
	//Adding the connection to the set
	_connections.insert(ptr);

	//Launching the connection read / write loop
	ptr->start();
	dout << _connections.size() << " active connections" << std::endl;
}

void Network::ConnectionManager::stop(IConnection::SharedPtr ptr)
{
    boost::mutex::scoped_lock   lock(_ioMutex);

	dout << "Delete connection" << std::endl;

	//Erasing the connection from the set
	_connections.erase(ptr);

	//Stopping the read / write loop by closing the socket
	ptr->stop();
	dout << _connections.size() << " active connections" << std::endl;
}

//Stop all connections
void Network::ConnectionManager::stop_all()
{
    boost::mutex::scoped_lock   lock(_ioMutex);

	for (auto co : _connections)
	{
		co->stop();
	}
	_connections.clear();
}
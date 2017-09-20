
#include "ConnectionManager.h"

using namespace Network;

Network::ConnectionManager::ConnectionManager()
{
}

Network::ConnectionManager::~ConnectionManager()
{
}

bool Network::ConnectionManager::add(IConnection::SharedPtr ptr)
{
	//Adding the connection to the set
	_connections.insert(ptr);

	//Launching the connection read / write loop
	ptr->start();
	return (false);
}

bool Network::ConnectionManager::stop(IConnection::SharedPtr ptr)
{
	//Erasing the connection from the set
	_connections.erase(ptr);

	//Stopping the read / write loop by closing the socket
	ptr->stop();
	return (false);
}

//Stop all connections
bool Network::ConnectionManager::stop_all()
{
	for (auto co : _connections)
	{
		co->stop();
	}
	_connections.clear();
	return (false);
}
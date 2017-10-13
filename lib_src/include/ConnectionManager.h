#pragma once

#include <set>
#include <boost/thread/mutex.hpp>
#include "IConnection.h"

namespace Network
{
	class ConnectionManager
	{
	private:
        boost::mutex                        _ioMutex;
		std::set<IConnection::SharedPtr>	_connections;

	public:
		ConnectionManager();
		virtual ~ConnectionManager();

		//We can't copy a connection manager
		//It would imply problems with the life management
		//of the connections
		ConnectionManager(ConnectionManager const &) = delete;
		ConnectionManager &operator=(ConnectionManager const &) = delete;

	public:
		void add(IConnection::SharedPtr);
		void stop(IConnection::SharedPtr);
		void stop_all();
	};
}
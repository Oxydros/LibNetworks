#pragma once

#include <set>
#include <boost/thread/mutex.hpp>
#include "IConnection.h"

namespace Network
{
    //! A Connection manager class
    /*!
     * Handle the life of the connections
     */
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
        /*!
         * Add a new connection
         */
		void add(IConnection::SharedPtr);

        /*!
         * Stop a connection
         */
		void stop(IConnection::SharedPtr);

        /*!
         * Stop all connections
         */
		void stop_all();
	};
}
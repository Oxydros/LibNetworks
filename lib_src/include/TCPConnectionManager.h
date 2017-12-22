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
	class TCPConnectionManager
	{
	private:
        boost::mutex                        _ioMutex;
		std::set<IConnection::SharedPtr>	_connections;

	public:
		TCPConnectionManager();
		~TCPConnectionManager();

		//We can't copy a connection manager
		//It would imply problems with the life management
		//of the connections
		TCPConnectionManager(TCPConnectionManager const &) = delete;
		TCPConnectionManager &operator=(TCPConnectionManager const &) = delete;

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
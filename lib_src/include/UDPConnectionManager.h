#pragma once

#include <set>
#include <boost/thread/mutex.hpp>
#include "UDPConnection.h"

namespace Network
{
    //! A Connection manager class
    /*!
     * Handle the life of the connections
     */
	class UDPConnectionManager
	{
	private:
        boost::asio::ip::udp::socket        &_socket;
        boost::asio::io_service::strand                 &_strand;
        PacketCallback                      &_callBack;
        boost::mutex                        _ioMutex;
		std::set<IConnection::SharedPtr>	_connections;
        UDPConnection::endpoint             _currentRemoteEndpoint;
        //Use in receiving
        boost::circular_buffer<char>        _readBuffer;
        std::vector<unsigned char>			_readActionBuffer;

	public:
        UDPConnectionManager(boost::asio::ip::udp::socket &socket,
                             boost::asio::io_service::strand &strand,
                             PacketCallback &callBack);
		~UDPConnectionManager();

		//We can't copy a connection manager
		//It would imply problems with the life management
		//of the connections
        UDPConnectionManager(UDPConnectionManager const &) = delete;
        UDPConnectionManager &operator=(UDPConnectionManager const &) = delete;

	public:
        /*!
         * Launch the IO for UDP sockets
         */
        void run();
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

    private:
        void processRead();
        IPacketConnection::SharedPtr  extractRemotePacketCo(boost::asio::ip::udp::endpoint &remoteEndpoint);
	};
}
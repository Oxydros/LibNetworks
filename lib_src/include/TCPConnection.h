#pragma once

#include <boost/thread/mutex.hpp>
#include "Debug.h"
#include "IConnection.h"
#include "ConnectionManager.h"
#include "PacketObserver.h"

namespace Network
{
    //! TCPConnection
    /*!
     * Implementation of a TCPConnection
     */
	class TCPConnection : public IConnection, public std::enable_shared_from_this<TCPConnection>
	{
	private:
		ConnectionManager					*_connectionManager;
		PacketObserver						&_observer;
		boost::asio::ip::tcp::socket		_socket;
		std::vector<unsigned char>			_buffer;
		std::vector<unsigned char>			_toSendBuffer;
		bool								_stopped;
        boost::mutex                        _ioMutex;

	public:
		explicit TCPConnection(boost::asio::ip::tcp::socket socket,
			PacketObserver &observer, ConnectionManager *manager = nullptr);
		virtual ~TCPConnection() = default;

	public:
        /*!
         * Start the TCP Connection
         */
		virtual void start();

        /*!
         * Stop the TCP Connection
         */
		virtual void stop();

        /*!
         * Send a packet
         * @param packet
         * @return
         */
		virtual bool sendPacket(IPacket const &packet);

	private:
        /*!
         * Check if we can write on the socket
         */
		void	checkWrite();

        /*!
         * Handle the read request
         * @param nbBytes
         */
		void	handleRead(size_t nbBytes);

        /*!
         * Handle the write request
         * @param ec
         */
		void 	handleWrite(boost::system::error_code ec);

        /*!
         * Process the read
         */
		void 	processRead();

        /*!
         * Process the write
         * @param ec
         */
		void 	processWrite(boost::system::error_code &ec);
	};
}
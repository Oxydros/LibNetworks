#pragma once

#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>
#include "Common.h"
#include "IConnection.h"
#include "TCPConnectionManager.h"

namespace Network
{
    //! TCPConnection
    /*!
     * Implementation of a TCPConnection
     */
	class TCPConnection : public IConnection, public std::enable_shared_from_this<TCPConnection>
	{
	private:
        boost::asio::strand                 &_strand;
		TCPConnectionManager				*_connectionManager;
		PacketObserver						&_callBack;
		boost::asio::ip::tcp::socket		_socket;
		bool								_stopped;
        boost::mutex                        _ioMutex;
        //Use in receiving
        boost::circular_buffer<char>        _readBuffer;
        std::vector<unsigned char>			_readActionBuffer;
        //Use in sending
        boost::circular_buffer<char>    	_toSendBuffer;

	public:
		explicit TCPConnection(boost::asio::strand &_strand,
                               boost::asio::ip::tcp::socket socket,
			PacketObserver &observer, TCPConnectionManager *manager = nullptr);
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
         * Handle the write request
         * @param ec
         */
		void 	handleWrite(boost::system::error_code ec);

        /*!
         * Process the read
         */
		void 	processRead();
	};
}
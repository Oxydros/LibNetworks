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
        boost::asio::io_service::strand                 &_strand;
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
		explicit TCPConnection(boost::asio::io_service::strand &_strand,
                               boost::asio::ip::tcp::socket socket,
			PacketObserver &observer, TCPConnectionManager *manager = nullptr);
		~TCPConnection() = default;

	public:
        /*!
         * Start the TCP Connection
         */
		void start() override;

        /*!
         * Stop the TCP Connection
         */
		void stop() override;

        /*!
         * Send a packet
         * @param packet
         * @return
         */
		bool sendPacket(IPacket const &packet) override;

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
#pragma once

#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp>
#include "Common.h"
#include "IPacketConnection.h"
#include "TCPConnectionManager.h"

namespace Network
{
    //! TCPConnection
    /*!
     * Implementation of a TCPConnection
     */
	class TCPPacketConnection : public IPacketConnection, public std::enable_shared_from_this<TCPPacketConnection>
	{
	private:
        boost::asio::io_service::strand                 &_strand;
		TCPConnectionManager				*_connectionManager;
		PacketCallback						_callBack;
		boost::asio::ip::tcp::socket		_socket;
		bool								_stopped;
        boost::mutex                        _ioMutex;
        //Use in receiving
        boost::circular_buffer<char>        _readBuffer;
        std::vector<unsigned char>			_readActionBuffer;
        //Use in sending
        boost::circular_buffer<char>    	_toSendBuffer;

	public:
		explicit TCPPacketConnection(boost::asio::io_service::strand &_strand,
                               boost::asio::ip::tcp::socket socket,
			PacketCallback &observer, TCPConnectionManager *manager = nullptr);
		~TCPPacketConnection() = default;

	public:
        /*!
         * Start the TCP Connection
         */
		void start() override;

        /*!
         * Stop the TCP Connection
         */
		void stop() override;

        bool isOpen() const override { return _socket.is_open(); };

        /*!
         * Send a packet
         * @param packet
         * @return
         */
		bool sendPacket(IPacket::SharedPtr packet) override;

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
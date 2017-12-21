#pragma once

#include <boost/thread/mutex.hpp>
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
		TCPConnectionManager					*_connectionManager;
		PacketObserver						&_callBack;
		boost::asio::ip::tcp::socket		_socket;
		bool								_stopped;
        boost::mutex                        _ioMutex;
        //Use in receiving
        std::vector<unsigned char>			_buffer;
        std::vector<unsigned char>			_finalBuffer;
        PacketSize                          _bytesSize;
        PacketSize                          _packetSize;
        PacketSize                          _packetRead;
        //Use in sending
        std::vector<unsigned char>			_toSendBuffer;

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
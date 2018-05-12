#pragma once

#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread/pthread/mutex.hpp>
#include "Common.h"
#include "IRawConnection.h"
#include "TCPConnectionManager.h"

namespace Network
{
    //! TCPConnection
    /*!
     * Implementation of a TCPConnection
     */
	class TCPRawConnection : public IRawConnection, public std::enable_shared_from_this<TCPRawConnection>
	{
	private:
        boost::asio::io_service::strand     &_strand;
		TCPConnectionManager				*_connectionManager;
        RawEndCallback 						_callBack;
		boost::asio::ip::tcp::socket		_socket;
		bool								_stopped;
		bool                                _expectRead{false};
		size_t                              _expectReadSize{0};
        boost::mutex                        _ioMutex;
        //Use in receiving
        Network::ByteBuffer     			_readBuffer;
        Network::ByteBuffer     			_readActionBuffer;
        //Use in sending
        Network::ByteBuffer    	            _toSendBuffer;

	public:
		explicit TCPRawConnection(boost::asio::io_service::strand &_strand,
                                   boost::asio::ip::tcp::socket socket,
                                   RawEndCallback observer,
                                   TCPConnectionManager *manager = nullptr);
		~TCPRawConnection() = default;

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
         * Send raw bytes
         * @param packet
         * @return
         */
        bool sendRawBytes(ByteBuffer bytes) override;

        void readRawBytes(size_t expectedSize) override;

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
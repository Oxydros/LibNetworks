#pragma once

#include "Client.h"
#include "TCPConnection.h"

namespace Network
{
    //! TCPClient
    /*!
     * Implementation of a TCPClient
     */
	class TCPClient : public Client
	{
	private:
		boost::asio::io_service			_io_service;
        TCPConnection::SharedPtr        _tcpConnection;
        boost::asio::signal_set			_signalRegister;

	public:
		explicit TCPClient();
		virtual ~TCPClient();

	public:
        /*!
         * Connect to a foreign TCP server
         * @param ip
         * @param port
         * @return
         */
		virtual bool		connect(std::string const &ip, std::string const &port);

        /*!
         * Disconnect from a server
         */
		virtual void		disconnect();

        /*!
         * Send a packet
         * @param p
         */
        virtual void        sendPacket(IPacket const &p);

        /*!
         * Launch the loop
         */
		virtual void		run();

    private:
        void	            handleAsyncWait();
	};
}
#pragma once

#include "Client.h"
#include "UDPConnectionManager.h"

namespace Network
{
    //! UDPClient
    /*!
     * Implementation of a UDPClient
     */
	class UDPClient : public Client
	{
	private:
		boost::asio::io_service			_io_service;
        boost::asio::strand              _strand;
        boost::asio::ip::udp::socket    _socket;
        boost::asio::ip::udp::endpoint  _endpoint;
        UDPConnectionManager             _manager;
        UDPConnection::SharedPtr        _mainCo;
        boost::asio::signal_set			_signalRegister;

	public:
		explicit UDPClient();
		virtual ~UDPClient();

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
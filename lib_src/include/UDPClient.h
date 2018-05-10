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
        boost::asio::io_service::strand              _strand;
        boost::asio::ip::udp::socket    _socket;
        boost::asio::ip::udp::endpoint  _endpoint;
        UDPConnectionManager             _manager;
        UDPConnection::SharedPtr        _mainCo;
        boost::asio::signal_set			_signalRegister;

	public:
		explicit UDPClient();
		~UDPClient() override;

	public:
        /*!
         * Connect to a foreign TCP server
         * @param ip
         * @param port
         * @return
         */
		bool		connect(std::string const &ip, std::string const &port) override;

        /*!
         * Disconnect from a server
         */
		void		disconnect() override;

        /*!
         * Send a packet
         * @param p
         */
        void        sendPacket(IPacket::SharedPtr p) override;

        /*!
         * Launch the loop
         */
		void		run() override;

    private:
        void	            handleAsyncWait();
	};
}
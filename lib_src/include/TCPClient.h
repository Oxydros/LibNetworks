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
        boost::asio::io_service::strand              _strand;
        TCPConnection::SharedPtr        _tcpConnection;
        boost::asio::signal_set			_signalRegister;

	public:
		explicit TCPClient();
		~TCPClient() override;

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

        void        receiveFile(std::string const &ip, std::string const &port, size_t fileSize,
                                std::vector<char> &fileData);

    private:
        void	            handleAsyncWait();
	};
}
#pragma once

#include "Client.h"
#include "TCPPacketConnection.h"
#include "TCPRawConnection.h"
#include "FileExchanger.h"

namespace Network
{
    //! TCPClient
    /*!
     * Implementation of a TCPClient
     */
	class TCPClient : public Client
	{
	private:
        boost::asio::io_service                     _io_service;
        boost::asio::io_service::strand             _strand;
        TCPPacketConnection::SharedPtr              _tcpPacketConnection;
        boost::asio::signal_set			            _signalRegister;
        FileExchanger                               _fileExchanger;

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
         * Send raw bytes
         * @param bytes
         * @return
         */
        void        sendFile(std::string const &ip, std::string const &port, ByteBuffer bytes);

		std::shared_ptr<FileExchanger::FileExchange>	prepareFileReception();
        void        receiveFile(std::shared_ptr<FileExchanger::FileExchange> fileExchange, size_t expectedSize,
                                Network::RawCallback callback);

        /*!
         * Launch the loop
         */
		void		run() override;

    private:
        void	            handleAsyncWait();
	};
}
#pragma once

#include "Common.h"
#include "IServer.h"
#include "TCPConnectionManager.h"
#include "TCPPacketConnection.h"

namespace Network
{
    //! TCPServer
    /*!
     * Implementation of a TCPServer
     */
	class TCPServer : public IServer
	{
	private:
		std::string const				&_serverIp;
		std::string const				&_serverPort;
		boost::asio::io_service         _io_service;
		PacketCallback					_callBack;
        boost::asio::io_service::strand _strand;
		boost::asio::ip::tcp::socket	_serverSocket;
		boost::asio::signal_set			_signalRegister;
		boost::asio::ip::tcp::acceptor	_acceptor;
		TCPConnectionManager			_connectionManager;

	public:
		explicit TCPServer(std::string const &ip,
			std::string const &serverPort);
		~TCPServer() override;

		//Can't copy a server
		TCPServer(TCPServer const &) = delete;
		TCPServer &operator=(TCPServer const &) = delete;

	public:
		bool    run() override;
        void    setCallback(PacketCallback &callback) override;
        void    setCallback(PacketCallback &&callback) override;

	private:
		void	handleAsyncWait();
		void	processAccept();
	};
}
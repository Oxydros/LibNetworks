#pragma once

#include "Common.h"
#include "IServer.h"
#include "ConnectionManager.h"
#include "TCPConnection.h"

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
		PacketObserver					_callBack;
		boost::asio::io_service			_io_service;
		boost::asio::ip::tcp::socket	_serverSocket;
		boost::asio::signal_set			_signalRegister;
		boost::asio::ip::tcp::acceptor	_acceptor;
		ConnectionManager				_connectionManager;

	public:
		explicit TCPServer(std::string const &ip,
			std::string const &serverPort);
		virtual ~TCPServer();

		//Can't copy a server
		TCPServer(TCPServer const &) = delete;
		TCPServer &operator=(TCPServer const &) = delete;

	public:
		virtual bool    run();
        virtual void    setCallback(PacketObserver &callback);
        virtual void    setCallback(PacketObserver &&callback);

	private:
		void	handleAsyncWait();
		void	processAccept();
	};
}
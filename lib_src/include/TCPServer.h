#pragma once

#include <boost/asio.hpp>

#include "IServer.h"
#include "ConnectionManager.h"
#include "TCPConnection.h"
#include "PacketObserver.h"

namespace Network
{
	class TCPServer : public IServer
	{
	private:
		std::string const				&_serverIp;
		std::string const				&_serverPort;
		PacketObserver					&_packetObserver;
		boost::asio::io_service			_io_service;
		boost::asio::ip::tcp::socket	_serverSocket;
		boost::asio::signal_set			_signalRegister;
		boost::asio::ip::tcp::acceptor	_acceptor;
		ConnectionManager				_connectionManager;

	public:
		explicit TCPServer(std::string const &ip,
			std::string const &serverPort,
			PacketObserver &observer);
		virtual ~TCPServer();

		//Can't copy a server
		TCPServer(TCPServer const &) = delete;
		TCPServer &operator=(TCPServer const &) = delete;

	public:
		virtual bool run();

	private:
		void	handleAsynWait();
		void	processAccept();
	};
}
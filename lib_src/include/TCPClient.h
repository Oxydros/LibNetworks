#pragma once

#include "Client.h"

namespace Network
{
	class TCPClient : public Client
	{
	private:
		boost::asio::io_service			_io_service;
		boost::asio::ip::tcp::socket	_socket;

	public:
		explicit TCPClient(PacketObserver &o);
		virtual ~TCPClient();

	public:
		virtual bool		connect(std::string const &ip, std::string const &port);
		virtual void		disconnect();
		virtual void		run();
	};
}
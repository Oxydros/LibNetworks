#pragma once

#include "Client.h"
#include "TCPConnection.h"

namespace Network
{
	class TCPClient : public Client
	{
	private:
		boost::asio::io_service			_io_service;
        TCPConnection::SharedPtr        _tcpConnection;
        boost::asio::signal_set			_signalRegister;

	public:
		explicit TCPClient(PacketObserver &o);
		virtual ~TCPClient();

	public:
		virtual bool		connect(std::string const &ip, std::string const &port);
		virtual void		disconnect();
        virtual void        sendPacket(IPacket const &p);
		virtual void		run();

    private:
        void	            handleAsyncWait();
	};
}
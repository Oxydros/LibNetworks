#pragma once

#include "Debug.h"
#include "IConnection.h"
#include "ConnectionManager.h"
#include "PacketObserver.h"

namespace Network
{
	class TCPConnection : public IConnection, public std::enable_shared_from_this<TCPConnection>
	{
	private:
		ConnectionManager					*_connectionManager;
		PacketObserver						&_observer;
		boost::asio::ip::tcp::socket		_socket;
		std::vector<unsigned char>			_buffer;
		std::vector<unsigned char>			_toSendBuffer;
		bool								_isWriting;
		bool								_stopped;

	public:
		explicit TCPConnection(boost::asio::ip::tcp::socket socket,
			PacketObserver &observer, ConnectionManager *manager = nullptr);
		virtual ~TCPConnection() = default;

	public:
		virtual void start();
		virtual void stop();
		virtual bool sendPacket(IPacket const &packet);

	private:
		void	checkWrite();
		void 	handleWrite(boost::system::error_code ec);
		void 	processRead();
		void 	processWrite(boost::system::error_code &ec);
	};
}
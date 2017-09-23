#pragma once

#include <vector>
#include <boost/asio.hpp>
#include "IConnection.h"
#include "PacketObserver.h"

namespace Network
{
	class ConnectionManager;

	class TCPConnection : public IConnection
	{
	private:
		ConnectionManager					&_connectionManager;
		PacketObserver						&_observer;
		boost::asio::ip::tcp::socket		_socket;
		std::vector<unsigned char>			_buffer;

	public:
		explicit TCPConnection(boost::asio::ip::tcp::socket socket,
			ConnectionManager &manager, PacketObserver &observer);
		virtual ~TCPConnection();

	public:
		virtual bool start();
		virtual bool stop();

	private:
		void process_read();
		void process_write();
	};
}
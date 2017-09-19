#pragma once

#include <boost/asio.hpp>
#include "IConnection.h"

namespace Network
{
	class TCPConnection : public IConnection
	{

	private:
		ConnectionInfo		*_connectionInfo;

	public:
		TCPConnection(boost::asio::io_service &io_service);
		virtual ~TCPConnection();

	public:
		bool start();
		bool stop();
		ConnectionInfo const &getConnectionInfo() const;
		bool write(std::string const &data);
		std::string read();
	};
}
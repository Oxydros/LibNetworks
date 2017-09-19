#pragma once

#include <boost/asio.hpp>
#include "IConnection.h"

namespace Network
{
	class UDPConnection : public IConnection
	{

	private:
		ConnectionInfo		*_connectionInfo;

	public:
		UDPConnection(boost::asio::io_service &io_service);
		virtual ~UDPConnection();

	public:
		bool start();
		bool stop();
		ConnectionInfo const &getConnectionInfo() const;
		bool write(std::string const &data);
		std::string read();
	};
}
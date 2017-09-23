#pragma once

#include <boost/asio.hpp>
#include "IConnection.h"

namespace Network
{
	class UDPConnection : public IConnection
	{

	private:

	public:
		UDPConnection(boost::asio::io_service &io_service);
		virtual ~UDPConnection();

	public:
		bool start();
		bool stop();
		bool write(std::string const &data);
		std::string read();
	};
}
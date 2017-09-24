#pragma once

#include <boost/asio.hpp>
#include "IConnection.h"

namespace Network
{
	class UDPConnection : public IConnection, public std::enable_shared_from_this<UDPConnection>
	{

	private:

	public:
		UDPConnection(boost::asio::io_service &io_service);
		virtual ~UDPConnection();

	public:
		virtual void start();
		virtual void stop();
		virtual bool sendPacket(IPacket const &packet);
	};
}
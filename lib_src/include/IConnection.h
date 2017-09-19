#pragma once

#include <string>
#include <memory>

namespace Network
{
	class IConnection;

	struct ConnectionInfo
	{
		std::string		ip;
		unsigned int	port;
		IConnection		*tcpConnection;
		IConnection		*udpConnection;
	};

	class IConnection
	{
	public:
		typedef std::shared_ptr<IConnection>	Ptr;

	public:
		IConnection() {}
		virtual ~IConnection() {}

	public:
		virtual bool start() = 0;
		virtual bool stop() = 0;
		virtual ConnectionInfo const &getConnectionInfo() const = 0;
		virtual bool write(std::string const &data) = 0;
		virtual std::string read() = 0;
	};
}
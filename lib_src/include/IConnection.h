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

	class IConnection : public std::enable_shared_from_this<IConnection>
	{
	public:
		typedef std::shared_ptr<IConnection>	SharedPtr;
		typedef std::unique_ptr<IConnection>	UniquePtr;

	public:
		IConnection() = default;
		virtual ~IConnection() = default;
		IConnection(IConnection const &) = delete;
		IConnection &operator=(IConnection const &) = delete;

	public:
		virtual bool start() = 0;
		virtual bool stop() = 0;
		virtual ConnectionInfo const &getConnectionInfo() const = 0;
	};
}
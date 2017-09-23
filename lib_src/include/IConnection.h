#pragma once

#include <string>
#include <memory>

namespace Network
{
	class IConnection;

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
	};
}
#pragma once

#include <string>
#include <memory>
#include "Common.h"
#include "IPacket.h"

namespace Network
{
	class IConnection;

	class IConnection
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
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual bool sendPacket(IPacket const &packet) = 0;
	};
}
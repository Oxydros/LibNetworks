#pragma once

#include <queue>
#include <boost\asio.hpp>
#include "PacketObserver.h"

namespace Network
{
	class Client
	{
	private:
		PacketObserver				&_packetObserver;

	public:
		explicit Client(PacketObserver &observer) : _packetObserver(observer)
		{}
		virtual ~Client() = default;

	public:
		virtual bool		connect(std::string const &ip, std::string const &port) = 0;
		virtual void		disconnect() = 0;
		virtual void		run() = 0;

	public:
		bool				sendPacket(IPacket const &packet);
		bool				changePacketObserver(PacketObserver &p);
	};
}
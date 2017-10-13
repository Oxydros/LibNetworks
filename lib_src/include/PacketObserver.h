#pragma once

#include "IPacket.h"
#include "IConnection.h"

namespace Network
{
	class PacketObserver
	{
    public:
		virtual void handlePacket(IConnection::SharedPtr from, IPacket const &packet) = 0;
	};
}
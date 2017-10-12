#pragma once

#include "IPacket.h"
#include "IConnection.h"

namespace Network
{
	class PacketObserver
	{
		virtual void handlePacket(IConnection const &from, IPacket const &packet) = 0;
	};
}
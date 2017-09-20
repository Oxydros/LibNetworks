#pragma once

#include "IConnection.h"

namespace Network
{
	class Packet;

	class PacketObserver
	{
		virtual Packet *handlePacket(IConnection const &from, Packet const &packet) = 0;
	};
}
#pragma once

#include "APacket.h"

namespace Network
{
	class UDPPacket : public APacket
	{
	public:
		explicit UDPPacket();
		virtual ~UDPPacket();

	public:
		virtual PacketBuffer getData() const;
		virtual bool setData(PacketBuffer const &buff);
	};
}
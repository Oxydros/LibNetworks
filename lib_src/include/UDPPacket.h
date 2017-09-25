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
		virtual std::size_t setData(PacketBuffer const &buff);
	};
}
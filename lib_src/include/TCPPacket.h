#pragma once

#include "APacket.h"

namespace Network
{
	class TCPPacket : public APacket
	{
	public:
		enum PacketType
		{
			CHAT,
			FILE,
			USER,
			ERROR
		};

	public:
		explicit				TCPPacket();
		virtual					~TCPPacket();

	public:
		virtual PacketBuffer	getData() const;
		virtual bool			setData(PacketBuffer const &buff);

	public:
		PacketType				getType() const;
		void					setType(PacketType newType);
	};
}
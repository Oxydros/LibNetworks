#pragma once

#include <vector>

namespace Network
{
	typedef std::vector<unsigned char> PacketBuffer;

	enum Protocol
	{
		TCP,
		UDP,
		UNKNOWN
	};

	class IPacket
	{
	public:
		IPacket() = default;
		virtual ~IPacket() = default;

	public:
		virtual Protocol getProtocol() const = 0;
		virtual void setProtocol(Protocol newPro) = 0;
		virtual PacketBuffer getData() const = 0;
		virtual bool setData(PacketBuffer const &buff) = 0;
	};
}
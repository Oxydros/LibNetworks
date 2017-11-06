#pragma once

#include <vector>

namespace Network
{
	typedef std::vector<unsigned char> PacketBuffer;

    //! Protocol enum
	enum Protocol
	{
		TCP,
		UDP,
		UNKNOWN
	};

    //! Packet Interface
    /*!
     * Specify what is a packet
     */
	class IPacket
	{
	public:
		IPacket() = default;
		virtual ~IPacket() = default;

	public:
		virtual Protocol getProtocol() const = 0;
		virtual void setProtocol(Protocol newPro) = 0;
		virtual PacketBuffer getData() const = 0;
		virtual std::size_t setData(PacketBuffer const &buff) = 0;
	};
}
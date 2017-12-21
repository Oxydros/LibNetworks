#pragma once

#include <TCPMessage.pb.h>
#include "APacket.h"

namespace Network
{
    //! TCPPacket
    /*!
     * Implementation of a TCPPacket
     */
	class TCPPacket : public APacket
	{
    public:
        typedef CubZPacket::PacketTCP::Type Type;

	private:
        CubZPacket::PacketTCP           _protobufPacket;

	public:
		explicit					    TCPPacket();
		virtual						    ~TCPPacket();

	public:
		virtual PacketBuffer		    getData() const;
		virtual std::size_t			    setData(PacketBuffer const &buff);

	public:
        Type                            getPacketType() const;

        CubZPacket::AuthMessage const   &getAuthMessage() const;
        CubZPacket::PingMessage const   &getPingMessage() const;
        CubZPacket::FileMessage const   &getFileMessage() const;

        CubZPacket::AuthMessage         *getMutableAuthMessage();
        CubZPacket::PingMessage         *getMutablePingMessage();
        CubZPacket::FileMessage         *getMutableFileMessage();

        CubZPacket::PacketTCP const      &getTCPPacket() const;

        void                            setType(Type t);
	};

	static const TCPPacket              EmptyTCPPacket;
}

std::ostream &operator<<(std::ostream &os, Network::TCPPacket const &packet);
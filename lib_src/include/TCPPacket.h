#pragma once

#include <TCPMessage.pb.h>
#include "APacket.h"

namespace Network
{
    //! TCPPacket
    /*!
     * Implementation of a TCPPacket
     */
	class TCPPacket : public APacket, public std::enable_shared_from_this<TCPPacket>
	{
    public:
        typedef CubZPacket::PacketTCP::Type Type;
        typedef std::shared_ptr<TCPPacket>  SharedPtr;

	private:
        CubZPacket::PacketTCP           _protobufPacket;

	public:
		explicit					    TCPPacket();
		    						    ~TCPPacket() override;

	public:
		ByteBuffer		            getData() const override;
		std::size_t			            setData(ByteBuffer const &buff) override;

	public:
        Type                            getPacketType() const;

        CubZPacket::PacketTCP const     &getTCPPacket() const;
        CubZPacket::AuthMessage const   &getAuthMessage() const;
        CubZPacket::PingMessage const   &getPingMessage() const;
        CubZPacket::FileMessage const   &getFileMessage() const;

        CubZPacket::PacketTCP           *getMutableTCPPacket();
        CubZPacket::AuthMessage         *getMutableAuthMessage();
        CubZPacket::PingMessage         *getMutablePingMessage();
        CubZPacket::FileMessage         *getMutableFileMessage();

        void                            setType(Type t);
	};

	static const TCPPacket              EmptyTCPPacket;
}

std::ostream &operator<<(std::ostream &os, Network::TCPPacket const &packet);
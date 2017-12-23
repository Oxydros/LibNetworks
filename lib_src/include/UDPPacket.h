#pragma once

#include <UDPMessage.pb.h>
#include "APacket.h"

namespace Network
{
    //! UDPPacket
    /*!
     * Implementation of a UDPPacket
     */
	class UDPPacket : public APacket
	{
    public:
        typedef CubZPacket::PacketUDP::Type Type;

    private:
        CubZPacket::PacketUDP          _protobufPacket;
	public:
		explicit UDPPacket();
		~UDPPacket() override;

	public:
		PacketBuffer getData() const override;
		std::size_t setData(PacketBuffer const &buff) override;

    public:
        void        setType(Type type);
        Type        getPacketType() const;

        CubZPacket::PacketUDP           *getMutableUDPPacket();

    public:
        void setString(std::string const &name);

    public:
        CubZPacket::PacketUDP const      &getUDPPacket() const;
	};
}

std::ostream &operator<<(std::ostream &os, Network::UDPPacket const &packet);

#include <Debug.h>
#include "UDPPacket.h"

using namespace Network;

Network::UDPPacket::UDPPacket():
        APacket(UDP), _protobufPacket()
{
    _protobufPacket.set_type(Type::PacketUDP_Type_UNKNOWN);
}

Network::UDPPacket::~UDPPacket()
{
}

ByteBuffer Network::UDPPacket::getData() const
{
    ByteBuffer    _protobufBuffer{};

    _protobufBuffer.resize(_protobufPacket.ByteSizeLong());
    _protobufPacket.SerializeToArray(_protobufBuffer.data(),
                                     _protobufPacket.ByteSize());
	UDPMSG("Get data on UDPPacket. Packet size: "
           << (_protobufBuffer.size()) << std::endl);
	UDPMSG(_protobufPacket.type() << std::endl);
	UDPMSG(_protobufPacket.DebugString() << std::endl);
    return (_protobufBuffer);
}

std::size_t Network::UDPPacket::setData(ByteBuffer const &buff)
{
	UDPMSG("Set data on UDPPacket(size: " << buff.size() << ")" << std::endl);
    _protobufPacket.ParseFromArray(buff.data(), buff.size());
	return (buff.size());
}

void Network::UDPPacket::setType(Network::UDPPacket::Type t)
{
    _protobufPacket.set_type(t);
}

Network::UDPPacket::Type Network::UDPPacket::getPacketType() const
{
    return (_protobufPacket.type());
}

CubZPacket::PacketUDP &Network::UDPPacket::getUDPPacket()
{
    return (_protobufPacket);
}

CubZPacket::PacketUDP const& Network::UDPPacket::getUDPPacket() const
{
    return (_protobufPacket);
}

std::ostream &operator<<(std::ostream &os, Network::UDPPacket const &packet)
{
    os << packet.getUDPPacket().DebugString();
    return (os);
}
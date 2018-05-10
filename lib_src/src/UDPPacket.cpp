
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

PacketBuffer Network::UDPPacket::getData() const
{
    PacketBuffer    _protobufBuffer{};

    _protobufBuffer.resize(_protobufPacket.ByteSizeLong());
    _protobufPacket.SerializeToArray(_protobufBuffer.data(),
                                     _protobufPacket.ByteSize());
    udpMsg << "Get data on UDPPacket. Packet size: "
           << (_protobufBuffer.size()) << std::endl;
    udpMsg << _protobufPacket.type() << std::endl;
    udpMsg << _protobufPacket.DebugString() << std::endl;
    return (_protobufBuffer);
}

std::size_t Network::UDPPacket::setData(PacketBuffer const &buff)
{
    udpMsg << "Set data on UDPPacket(size: " << buff.size() << ")" << std::endl;
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

CubZPacket::PacketUDP* Network::UDPPacket::getMutableUDPPacket()
{
    return (&_protobufPacket);
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
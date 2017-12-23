
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
    dout << "Get data on UDPPacket. Packet size: "
         << (_protobufBuffer.size()) << std::endl;
    dout << _protobufPacket.DebugString() << std::endl;
    return (_protobufBuffer);
}

std::size_t Network::UDPPacket::setData(PacketBuffer const &buff)
{
    dout << "Set data on UDPPacket(size: " << buff.size() << ")" << std::endl;
    _protobufPacket.ParseFromArray(buff.data(), buff.size());
	return (buff.size());
}

void Network::UDPPacket::setString(std::string const &name)
{
    _protobufPacket.set_info(name);
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
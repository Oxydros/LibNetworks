#include <Debug.h>
#include <TCPPacket.h>

using namespace Network;

Network::TCPPacket::TCPPacket()
	: APacket(TCP), _protobufPacket()
{
    _protobufPacket.set_type(CubZPacket::PacketTCP::UNKNOWN);
}

Network::TCPPacket::~TCPPacket()
{
}

ByteBuffer Network::TCPPacket::getData() const
{
    ByteBuffer	_finalBuffer{};

    _finalBuffer.resize(_protobufPacket.ByteSizeLong());
    _protobufPacket.SerializeToArray(_finalBuffer.data(),
                                     _protobufPacket.ByteSize());
	TCPMSG("Get data on TCPPPacket. Packet size: "
         << (_finalBuffer.size()) << std::endl);
    TRACE(_protobufPacket.DebugString() << std::endl);
    return (_finalBuffer);
}

std::size_t Network::TCPPacket::setData(ByteBuffer const &buff)
{
	TRACE("Set data on TCPPacket(size: " << buff.size() << ")" << std::endl);
    bool success{_protobufPacket.ParseFromArray(buff.data(), buff.size())};
    if (!success)
        TRACE("Couldn't parse protobuf packet !" << std::endl);
	TRACE("DEBUG PROTOBUF PRINT " <<_protobufPacket.DebugString() << std::endl);
	return (buff.size());
}

void Network::TCPPacket::setType(Type t)
{
    _protobufPacket.set_type(t);
}

Network::TCPPacket::Type Network::TCPPacket::getPacketType() const
{
    return (_protobufPacket.type());
}

CubZPacket::PacketTCP &Network::TCPPacket::getTCPPacket()
{
    return (_protobufPacket);
}

CubZPacket::PacketTCP const      &Network::TCPPacket::getTCPPacket() const
{
    return (_protobufPacket);
}

std::ostream &operator<<(std::ostream &os, Network::TCPPacket const &packet)
{
    os << packet.getTCPPacket().DebugString();
    return (os);
}

#include "UDPPacket.h"

using namespace Network;

Network::UDPPacket::UDPPacket()
{
}

Network::UDPPacket::~UDPPacket()
{
}

PacketBuffer Network::UDPPacket::getData() const
{
	return PacketBuffer();
}

std::size_t Network::UDPPacket::setData(PacketBuffer const & buff)
{
	return (0);
}
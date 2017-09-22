
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

bool Network::UDPPacket::setData(PacketBuffer const & buff)
{
	return false;
}
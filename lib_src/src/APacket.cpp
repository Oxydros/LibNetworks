#include "APacket.h"

using namespace Network;

Network::APacket::APacket(Protocol prot)
	: _prot(prot)
{
}

Network::APacket::~APacket()
{
}

Protocol Network::APacket::getProtocol() const
{
	return (_prot);
}

void Network::APacket::setProtocol(Protocol newProt)
{
	_prot = newProt;
}
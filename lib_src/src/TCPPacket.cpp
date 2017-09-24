#include <Debug.h>
#include <TCPPacket.h>

using namespace Network;

Network::TCPPacket::TCPPacket()
	: _type(UNKNOWN), _fileData()
{
}

Network::TCPPacket::~TCPPacket()
{
}

PacketBuffer Network::TCPPacket::getData() const
{
	dout << "Get data on TCPPacket" << std::endl;
	return PacketBuffer();
}

bool Network::TCPPacket::setData(PacketBuffer const &buff)
{
	dout << "Set data on TCPPacket(size: " << buff.size() << ")" << std::endl;
	return false;
}

TCPPacket::PacketType Network::TCPPacket::getType() const
{
	return (_type);
}

void Network::TCPPacket::setType(PacketType newType)
{
	_type = newType;
}

std::vector<unsigned char> Network::TCPPacket::getFileData() const
{
	return (_fileData);
}
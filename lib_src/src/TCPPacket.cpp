#include <Debug.h>
#include <TCPPacket.h>
#include <cstring>

using namespace Network;

Network::TCPPacket::TCPPacket()
	: APacket(), _protobufPacket(), _fileData()
{
    _protobufPacket.set_type(TCPMessage::TCPPacket::UNKNOWN);
}

Network::TCPPacket::~TCPPacket()
{
}

PacketBuffer Network::TCPPacket::getData() const
{
	PacketBuffer	_finalBuffer;
	PacketBuffer	_textBuffer;
	std::int32_t 	_bufferSize = 0;

	_textBuffer.resize(_protobufPacket.ByteSizeLong());
    _protobufPacket.SerializeToArray(_textBuffer.data(),
                                     _protobufPacket.ByteSize());

	_bufferSize += _protobufPacket.ByteSize();
	_finalBuffer.resize(_bufferSize + sizeof(_bufferSize));
	std::memcpy(_finalBuffer.data(), &_bufferSize, sizeof(_bufferSize));
	std::memcpy(_finalBuffer.data() + sizeof(_bufferSize),
				_textBuffer.data(),
				_textBuffer.size());

	dout << "Get data on TCPPacket. Packet size: "
		 << _bufferSize << " Real Size: "
		 << _finalBuffer.size() << std::endl;
	dout << _protobufPacket.DebugString() << std::endl;
	return _finalBuffer;
}

std::size_t Network::TCPPacket::setData(PacketBuffer const &buff)
{
	dout << "Set data on TCPPacket(size: " << buff.size() << ")" << std::endl;

	PacketBuffer	_textBuffer;

    _protobufPacket.ParseFromArray(buff.data(), buff.size());
	dout << "DEBUG PROTOBUF PRINT " <<_protobufPacket.DebugString() << std::endl;
	return (buff.size());
}

std::vector<unsigned char> Network::TCPPacket::getFileData() const
{
	return (_fileData);
}

Network::TCPPacket::Type Network::TCPPacket::getPacketType() const
{
    return (_protobufPacket.type());
}

TCPMessage::AuthMessage     Network::TCPPacket::getAuthMessage() const
{
    return (_protobufPacket.authmessage());
}

TCPMessage::PingMessage     Network::TCPPacket::getPingMessage() const
{
    return (_protobufPacket.pingmessage());
}

TCPMessage::FileMessage     Network::TCPPacket::getFileMessage() const
{
    return (_protobufPacket.filemessage());
}

TCPMessage::AuthMessage     *Network::TCPPacket::getMutableAuthMessage()
{
    return (_protobufPacket.mutable_authmessage());
}

TCPMessage::PingMessage     *Network::TCPPacket::getMutablePingMessage()
{
    return (_protobufPacket.mutable_pingmessage());
}

TCPMessage::FileMessage     *Network::TCPPacket::getMutableFileMessage()
{
    return (_protobufPacket.mutable_filemessage());
}
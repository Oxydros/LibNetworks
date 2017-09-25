#include <Debug.h>
#include <TCPPacket.h>
#include <cstring>

using namespace Network;

Network::TCPPacket::TCPPacket()
	: _type(UNKNOWN), _textProtobuf(), _fileData()
{
	_textProtobuf.set_message("Message de test");
	_textProtobuf.set_type(test::TestMessage::OUI);
}

Network::TCPPacket::~TCPPacket()
{
}

PacketBuffer Network::TCPPacket::getData() const
{
	PacketBuffer	_finalBuffer;
	PacketBuffer	_textBuffer;
	std::int32_t 	_bufferSize = 0;

	_textBuffer.resize(_textProtobuf.ByteSizeLong());
	_textProtobuf.SerializeToArray(_textBuffer.data(),
								   _textProtobuf.ByteSize());

	_bufferSize += _textProtobuf.ByteSize();
	_finalBuffer.resize(_bufferSize + sizeof(_bufferSize));
	std::memcpy(_finalBuffer.data(), &_bufferSize, sizeof(_bufferSize));
	std::memcpy(_finalBuffer.data() + sizeof(_bufferSize),
				_textBuffer.data(),
				_textBuffer.size());

	dout << "Get data on TCPPacket. Packet size: "
		 << _bufferSize << " Real Size: "
		 << _finalBuffer.size() << std::endl;
	dout << _textProtobuf.message() << std::endl;
	return _finalBuffer;
}

std::size_t Network::TCPPacket::setData(PacketBuffer const &buff)
{
	dout << "Set data on TCPPacket(size: " << buff.size() << ")" << std::endl;

	std::int32_t 	_bufferSize = 0;
	PacketBuffer	_textBuffer;

	//std::memcpy(&_bufferSize, buff.data(), sizeof(_bufferSize));
	//dout << "Real size " <<_bufferSize << std::endl;
	//_textProtobuf.ParseFromArray(buff.data() + sizeof(_bufferSize), _bufferSize);
	_textProtobuf.ParseFromArray(buff.data(), buff.size());
	dout << _textProtobuf.message() << std::endl;
	return (buff.size());
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
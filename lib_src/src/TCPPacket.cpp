#include <Debug.h>
#include <TCPPacket.h>
#include <cstring>
#include <Common.h>

using namespace Network;

Network::TCPPacket::TCPPacket()
	: APacket(TCP), _protobufPacket(), _fileData()
{
    _protobufPacket.set_type(CubZPacket::PacketTCP::UNKNOWN);
}

Network::TCPPacket::~TCPPacket()
{
}

PacketBuffer Network::TCPPacket::getData() const
{
	PacketBuffer	_finalBuffer;
	PacketBuffer	_protobufBuffer;
	std::int32_t 	_protobufPacketSize = 0;
    std::int32_t    _fileSize = 0;
    std::int32_t    _totalPacketSize = 0;

    _fileSize = static_cast<int32_t>(_fileData.size());

    _protobufBuffer.resize(_protobufPacket.ByteSizeLong());
    _protobufPacket.SerializeToArray(_protobufBuffer.data(),
                                     _protobufPacket.ByteSize());

    _protobufPacketSize = _protobufPacket.ByteSize();
    _totalPacketSize = _protobufPacketSize + _fileSize
                       + sizeof(_protobufPacketSize) + sizeof(_fileSize);
    //Resize final buffer
	_finalBuffer.resize(_totalPacketSize);

    //Write the size of the protobuf packet in the beginning
	std::memcpy(_finalBuffer.data(), &_protobufPacketSize, sizeof(_protobufPacketSize));

    //Write the size of the file just after
    std::memcpy(_finalBuffer.data() + sizeof(_protobufPacketSize),
                &_fileSize, sizeof(_fileSize));

    //Write the protobuf packet
	std::memcpy(_finalBuffer.data() +sizeof(_protobufPacketSize) + sizeof(_fileSize),
                _protobufBuffer.data(),
                _protobufBuffer.size());
    //Write the file
    std::memcpy(_finalBuffer.data() + sizeof(_protobufPacketSize)
                + sizeof(_fileSize) + _protobufPacketSize,
                _fileData.data(),
                _fileData.size());

	dout << "Get data on TCPPacket. Packet size: "
		 << (_totalPacketSize) << " Real Size: "
		 << _finalBuffer.size() << std::endl;
	dout << _protobufPacket.DebugString() << std::endl;
	return _finalBuffer;
}

std::size_t Network::TCPPacket::setData(PacketBuffer const &buff)
{
	dout << "Set data on TCPPacket(size: " << buff.size() << ")" << std::endl;

    std::int32_t 	_protobufPacketSize = 0;
    std::int32_t    _fileSize = 0;
	PacketBuffer	_textBuffer;

    //Deserialize size of protobuf packet
    std::memcpy(&_protobufPacketSize, buff.data(), sizeof(_protobufPacketSize));

    //Deserialize size of file
    std::memcpy(&_fileSize, buff.data() + sizeof(_protobufPacketSize), sizeof(_fileSize));

    //Deserialize protobuf packet
    _protobufPacket.ParseFromArray(buff.data() + sizeof(_protobufPacketSize)
            + sizeof(_fileSize), _protobufPacketSize);
    //Deserialize file
    _fileData.clear();
    _fileData.resize(_fileSize);
    std::memcpy(_fileData.data(), buff.data() + sizeof(_protobufPacketSize) +
            sizeof(_fileSize) + _protobufPacketSize, _fileSize);

	dout << "DEBUG PROTOBUF PRINT " <<_protobufPacket.DebugString() << std::endl;
	return (buff.size());
}

void Network::TCPPacket::setFileData(std::vector<unsigned char> const &fileData)
{
    _fileData = fileData;
}

void Network::TCPPacket::setFileData(char *data, size_t size)
{
    _fileData.clear();
    _fileData.resize(size);
    std::memcpy(_fileData.data(), data, size);
}

void Network::TCPPacket::setType(Type t)
{
    _protobufPacket.set_type(t);
}

std::vector<unsigned char> Network::TCPPacket::getFileData() const
{
	return (_fileData);
}

Network::TCPPacket::Type Network::TCPPacket::getPacketType() const
{
    return (_protobufPacket.type());
}

CubZPacket::AuthMessage const     &Network::TCPPacket::getAuthMessage() const
{
    return (_protobufPacket.authmessage());
}

CubZPacket::PingMessage const     &Network::TCPPacket::getPingMessage() const
{
    return (_protobufPacket.pingmessage());
}

CubZPacket::FileMessage const     &Network::TCPPacket::getFileMessage() const
{
    return (_protobufPacket.filemessage());
}

CubZPacket::AuthMessage     *Network::TCPPacket::getMutableAuthMessage()
{
    return (_protobufPacket.mutable_authmessage());
}

CubZPacket::PingMessage     *Network::TCPPacket::getMutablePingMessage()
{
    return (_protobufPacket.mutable_pingmessage());
}

CubZPacket::FileMessage     *Network::TCPPacket::getMutableFileMessage()
{
    return (_protobufPacket.mutable_filemessage());
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
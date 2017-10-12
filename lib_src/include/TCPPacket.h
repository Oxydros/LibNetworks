#pragma once

#include "TCPMessage.pb.h"
#include "APacket.h"

namespace Network
{
	class TCPPacket : public APacket
	{
    public:
        typedef TCPMessage::TCPPacket::Type Type;

	private:
		TCPMessage::TCPPacket           _protobufPacket;
		std::vector<unsigned char>		_fileData;

	public:
		explicit					TCPPacket();
		virtual						~TCPPacket();

	public:
		virtual PacketBuffer		getData() const;
		virtual std::size_t			setData(PacketBuffer const &buff);

	public:
		std::vector<unsigned char>	getFileData() const;
        Type                        getPacketType() const;

        TCPMessage::AuthMessage     getAuthMessage() const;
        TCPMessage::PingMessage     getPingMessage() const;
        TCPMessage::FileMessage     getFileMessage() const;
	};

	static const TCPPacket          EmptyTCPPacket;
}
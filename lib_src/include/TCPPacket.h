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
		explicit					    TCPPacket();
		virtual						    ~TCPPacket();

	public:
		virtual PacketBuffer		    getData() const;
		virtual std::size_t			    setData(PacketBuffer const &buff);

	public:
		std::vector<unsigned char>	    getFileData() const;
        Type                            getPacketType() const;

        TCPMessage::AuthMessage const   &getAuthMessage() const;
        TCPMessage::PingMessage const   &getPingMessage() const;
        TCPMessage::FileMessage const   &getFileMessage() const;

        TCPMessage::AuthMessage         *getMutableAuthMessage();
        TCPMessage::PingMessage         *getMutablePingMessage();
        TCPMessage::FileMessage         *getMutableFileMessage();

        TCPMessage::TCPPacket const      &getTCPPacket() const;

        void                            setType(Type t);
	};

	static const TCPPacket              EmptyTCPPacket;
}

std::ostream &operator<<(std::ostream &os, Network::TCPPacket const &packet);
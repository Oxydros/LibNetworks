#pragma once

#include "test.pb.h"
#include "APacket.h"

namespace Network
{
	class TCPPacket : public APacket
	{
	public:
		enum PacketType
		{
			CHAT,
			FILE,
			AUTH,
			PING,
			USER,
			UNKNOWN
		};

	private:
		PacketType						_type;
		test::TestMessage				_textProtobuf;
		std::vector<unsigned char>		_fileData;

	public:
		explicit					TCPPacket();
		virtual						~TCPPacket();

	public:
		virtual PacketBuffer		getData() const;
		virtual std::size_t			setData(PacketBuffer const &buff);

	public:
		PacketType					getType() const;
		void						setType(PacketType newType);
		std::vector<unsigned char>	getFileData() const;
	};

	static const TCPPacket EmptyTCPPacket;
}
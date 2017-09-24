#pragma once

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
		std::vector<unsigned char>		_fileData;

	public:
		explicit					TCPPacket();
		virtual						~TCPPacket();

	public:
		virtual PacketBuffer		getData() const;
		virtual bool				setData(PacketBuffer const &buff);

	public:
		PacketType					getType() const;
		void						setType(PacketType newType);
		std::vector<unsigned char>	getFileData() const;
	};

	static const TCPPacket EmptyTCPPacket;
}
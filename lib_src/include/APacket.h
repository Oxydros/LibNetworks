#pragma once

#include "IPacket.h"

namespace Network
{
	class APacket : public IPacket
	{
	private:
		Protocol	_prot;

	public:
		explicit APacket(Protocol prot = UNKNOWN);
		virtual ~APacket();
		
	public:
		virtual Protocol getProtocol() const;
		virtual void setProtocol(Protocol newProt);
	};
}
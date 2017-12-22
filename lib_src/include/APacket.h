#pragma once

#include "IPacket.h"

namespace Network
{
    /*! Abstract class implementing IPacket */
	class APacket : public IPacket
	{
	private:
		Protocol	_prot;

	public:
		explicit APacket(Protocol prot = UNKNOWN);
		virtual ~APacket();
		
	public:
        /*!
         * Get the protocol
         * @return Network::Protocol
         */
		Protocol getProtocol() const override;

        /*!
         * Set the protocol
         * @param Network::Protocol newProt
         * @return void
         */
		void setProtocol(Protocol newProt) override;
	};
}
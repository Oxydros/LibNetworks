#pragma once

#include <vector>
#include "Common.h"

namespace Network
{
    //! Protocol enum
	enum Protocol
	{
		TCP,
		UDP,
		UNKNOWN
	};

    //! Packet Interface
    /*!
     * Specify what is a packet
     */
	class IPacket
	{
    public:
        typedef std::shared_ptr<IPacket>    SharedPtr;

	public:
		IPacket() = default;
		virtual ~IPacket() = default;

	public:
        /*!
         * Return the protocol of the packet
         * @return Network::Protocol
         */
		virtual Protocol getProtocol() const = 0;

        /*!
         * Set the protocol used for this packet
         * @param Network::Protocol newPro
         */
		virtual void setProtocol(Protocol newPro) = 0;

        /*!
         * Get the binary representation of the packet
         * @return Network::PacketBuffer
         */

		virtual Network::PacketBuffer getData() const = 0;

        /*!
         * Set the binary representaton of the packet
         * @param buff
         * @return
         */
		virtual std::size_t setData(PacketBuffer const &buff) = 0;
	};
}
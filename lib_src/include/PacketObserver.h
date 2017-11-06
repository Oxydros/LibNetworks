#pragma once

#include "IPacket.h"
#include "IConnection.h"

namespace Network
{
    //! Packet Observer
    /*!
     * Class to implement to receive callbacks from the
     * servers and clients
     */
	class PacketObserver
	{
    public:
        /*!
         * Callback
         * @param from
         * @param packet
         */
		virtual void handlePacket(IConnection::SharedPtr from, IPacket const &packet) = 0;
	};
}
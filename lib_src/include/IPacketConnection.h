#pragma once

#include <string>
#include <memory>
#include "IConnection.h"

namespace Network
{
    //! Connection Interface
    /*!
     * Specify what is a connection
     */
	class IPacketConnection : public IConnection
	{
	public:
		typedef std::shared_ptr<IPacketConnection>	SharedPtr;
		typedef std::unique_ptr<IPacketConnection>	UniquePtr;

	public:
        IPacketConnection() = default;
		virtual ~IPacketConnection() = default;
        IPacketConnection(IPacketConnection const &) = delete;
        IPacketConnection &operator=(IPacketConnection const &) = delete;

	public:
        /*!
         * Send a packet
         * @param packet
         * @return true or false if succeed
         */
		virtual bool sendPacket(IPacket::SharedPtr packet) = 0;
	};
}
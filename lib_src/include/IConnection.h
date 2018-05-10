#pragma once

#include <string>
#include <memory>
#include "IPacket.h"

namespace Network
{
    //! Connection Interface
    /*!
     * Specify what is a connection
     */
	class IConnection
	{
	public:
		typedef std::shared_ptr<IConnection>	SharedPtr;
		typedef std::unique_ptr<IConnection>	UniquePtr;

	public:
		IConnection() = default;
		virtual ~IConnection() = default;
		IConnection(IConnection const &) = delete;
		IConnection &operator=(IConnection const &) = delete;

	public:
        /*!
         * Start the connection loop
         */
		virtual void start() = 0;

        /*!
         * Stop the connection / Disconnect socket
         */
		virtual void stop() = 0;

        /*!
         * Send a packet
         * @param packet
         * @return true or false if succeed
         */
		virtual bool sendPacket(IPacket::SharedPtr packet) = 0;
	};
}
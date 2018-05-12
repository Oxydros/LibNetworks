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
	class IRawConnection : public IConnection
	{
	public:
		typedef std::shared_ptr<IRawConnection>	SharedPtr;
		typedef std::unique_ptr<IRawConnection>	UniquePtr;

	public:
        IRawConnection() = default;
		virtual ~IRawConnection() = default;
        IRawConnection(IPacketConnection const &) = delete;
        IRawConnection &operator=(IRawConnection const &) = delete;

	public:
        /*!
         * Send a raw bytes
         * @param packet
         * @return true or false if succeed
         */
		virtual bool sendRawBytes(ByteBuffer bytes) = 0;
        virtual void readRawBytes(size_t expectedSize) = 0;
	};
}
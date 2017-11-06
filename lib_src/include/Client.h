#pragma once

#include <queue>
#include "Common.h"
#include "PacketObserver.h"

namespace Network
{
    /*! Abstract Client class */
	class Client
	{
	protected:
		PacketObserver				&_packetObserver;

	public:
		explicit Client(PacketObserver &observer)
                : _packetObserver(observer)
		{}
		virtual ~Client() = default;

	public:
        /*!
         * Connect to a foreign server
         * @param ip
         * @param port
         * @return true or false if connected
         */
		virtual bool		connect(std::string const &ip, std::string const &port) = 0;

        /*!
         * Disconnect from a foreign server
         */
		virtual void		disconnect() = 0;

        /*!
         * Send a packet
         * @param p
         */
        virtual void        sendPacket(IPacket const &p) = 0;

        /*!
         * Launch the client loop
         */
		virtual void		run() = 0;

	public:
        /*!
         * Change the packetObserver
         * @see Network::PacketObserver
         * @param p
         * @return
         */
		bool				changePacketObserver(PacketObserver &p)
        {
            _packetObserver = p;
            return (false);
        }
	};
}
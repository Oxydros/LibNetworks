#pragma once

#include <queue>
#include "Common.h"

namespace Network
{
    /*! Abstract Client class */
	class Client
	{
    public:
	protected:
        PacketObserver  _callBack;

	public:
		explicit Client()
                : _callBack()
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

        void                setCallback(PacketObserver &o)
        {
            _callBack = std::move(o);
        }

        void                setCallback(PacketObserver &&o)
        {
            _callBack = o;
        }
	};
}
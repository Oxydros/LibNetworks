#pragma once

#include <queue>
#include <boost/thread.hpp>
#include "Common.h"
#include "IPacket.h"

namespace Network
{
    /*! Abstract Client class */
	class Client
	{
	protected:
        PacketCallback                  _packetCallBack{};
        bool                            _isAsync;
        std::unique_ptr<boost::thread>  _thread;

	public:
		explicit Client()
                : _packetCallBack()
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
        virtual void        sendPacket(IPacket::SharedPtr p) = 0;

        /*!
         * Launch the client loop (block)
         */
		virtual void		run() = 0;

		void        async_run() {
            _isAsync = true;
            _thread = std::make_unique<boost::thread>([this](){ this->run(); });
		};

        void        wait() {
            if (_isAsync)
                _thread->join();
        };

        bool        isAsync() const { return (_isAsync); };

	public:
        /*!
         * Change the packetObserver
         * @see Network::PacketObserver
         * @param p
         * @return
         */

        void                setPacketCallback(PacketCallback &o)
        {
            _packetCallBack = std::move(o);
        }

        void                setPacketCallback(PacketCallback &&o)
        {
            _packetCallBack = o;
        }
	};
}
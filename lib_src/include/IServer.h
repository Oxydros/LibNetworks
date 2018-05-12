#pragma once

#include <boost/thread.hpp>
#include "Common.h"

namespace Network
{
    //! Server Interface
    /*!
     * Specify what is a Server
     */
	class IServer
	{
	private:
	    bool                            _isAsync{false};
	    std::unique_ptr<boost::thread>  _thread;
	public:
		IServer() {}
		virtual ~IServer() {}
		
	public:
        /*!
         * Launch server loop
         * @return true or false if it succeed
         */
		virtual bool	run() = 0;

		void    async_run(){
            _isAsync = true;
            _thread = std::make_unique<boost::thread>([this](){ this->run(); });
		}

		void    wait(){
		    if (isAsync())
		        _thread->join();
		}
		bool    isAsync() const { return (_isAsync); }


        virtual void    setCallback(PacketCallback &&callback) = 0;
        virtual void    setCallback(PacketCallback &callback) = 0;
	};
}
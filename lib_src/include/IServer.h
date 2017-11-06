#pragma once

namespace Network
{
    //! Server Interface
    /*!
     * Specify what is a Server
     */
	class IServer
	{
	public:
		IServer() {}
		virtual ~IServer() {}
		
	public:
        /*!
         * Launch server loop
         * @return true or false if it succeed
         */
		virtual bool	run() = 0;
	};
}
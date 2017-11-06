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
		virtual bool	run() = 0;
	};
}
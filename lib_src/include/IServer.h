#pragma once

namespace Network
{
	class IServer
	{
	public:
		IServer() {}
		virtual ~IServer() {}
		
	public:
		virtual bool	run() = 0;
	};
}
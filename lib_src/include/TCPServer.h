#pragma once

#include "IServer.h"

namespace Network
{
	class TCPServer : public IServer
	{
	public:
		TCPServer();
		virtual ~TCPServer();

	public:
		virtual bool run();
	};
}
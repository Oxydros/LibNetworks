#pragma once

#include <set>
#include "IConnection.h"

namespace Network
{
	class ConnectionManager
	{
	private:
		std::set<IConnection::Ptr>	_connections;

	public:
		ConnectionManager();
		virtual ~ConnectionManager();

	public:
		bool add(IConnection::Ptr);
		bool stop(IConnection::Ptr);
		bool stop_all();
	};
}
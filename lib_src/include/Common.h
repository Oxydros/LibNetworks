#pragma once

#ifdef  _DEBUG
# define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#endif

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>

#include "IConnection.h"
#include "IPacket.h"

namespace Network
{
    typedef std::int32_t                PacketSize;
    typedef std::function<void(Network::IConnection::SharedPtr, Network::IPacket const &)>  PacketObserver;
}
#pragma once

#ifdef  _DEBUG
# define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#endif

#define BUFFER_SIZE     (1024)

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>

#include "Debug.h"
#include "IConnection.h"
#include "IPacket.h"

namespace Network
{
    typedef std::int32_t                PacketSize;

    typedef std::function<void(Network::IConnection::SharedPtr, Network::IPacket const &)>  PacketObserver;
}
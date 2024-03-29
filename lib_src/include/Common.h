#pragma once
#ifndef COMMON_INCLUDE_H
# define COMMON_INCLUDE_H

//#if defined(_DEBUG) || defined(CUBZ_NETWORK_DEBUG)
# define BOOST_ASIO_ENABLE_HANDLER_TRACKING
//#endif
#define MAX_BUFFER_SIZE     (2048)
#define READ_SIZE           (512)
#define HEADER_TYPE         int
#define HEADER_SIZE         (sizeof(HEADER_TYPE))

#ifndef MAX
# define MAX(x, y)           ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
# define MIN(x, y)           ((x) < (y) ? (x) : (y))
#endif

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>
#include "Debug.h"

namespace Network
{
    class IConnection;
    class IPacketConnection;
    class IRawConnection;
    class IPacket;

    typedef HEADER_TYPE                 PacketSize;
    typedef std::vector<char>           ByteBuffer;

    typedef std::function< void(std::shared_ptr<IPacketConnection>, std::shared_ptr<IPacket> ) >    PacketCallback;
    typedef std::function < void(std::shared_ptr<IRawConnection>, ByteBuffer)>                      RawEndCallback;
    typedef std::function< void(ByteBuffer ) >                                                      RawCallback;
}
#endif

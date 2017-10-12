#pragma once

#ifdef  _DEBUG
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#endif

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
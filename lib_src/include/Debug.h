#pragma once

#include <iostream>

#ifdef _DEBUG
# define dout std::cout << __FILE__ << "(" << __LINE__ << ") DEBUG: "
#else
# define dout 0 && std::cout
#endif

#define tcpMsg dout << "TCP: "
#define udpMsg dout << "UDP: "
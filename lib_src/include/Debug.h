#pragma once

#include <iostream>

#if defined(_DEBUG) || defined(CUBZ_NETWORK_DEBUG)

# if defined (__WIN32__)
	#define TRACE(x) \
			{std::stringstream s;  s << __FILE__ << "(" << __LINE__ << ") DEBUG: " << x;\
            ::OutputDebugStringA(s.str().c_str());}
# else
#  define TRACE(x) std::cout << __FILE__ << "(" << __LINE__ << ") DEBUG: " << x
# endif

# else
# define TRACE(x) 0 && std::cout
#endif

#define TCPMSG(x) TRACE("TCP: " << x)
#define UDPMSG(x) TRACE("UDP: " << x)


#include "LoggerImpl.h"
#include <cstdint>


#ifdef _WIN32

#include <windows.h>


#else
#include <unistd.h>
#endif


std::uint32_t Logify::Logger::Impl::getPID()
{
#ifdef _WIN32
	// On Windows, use GetCurrentProcessId() to retrieve the process ID.
	return static_cast<std::uint32_t>(GetCurrentProcessId());
#else
	// On Unix-like systems, use getpid() to retrieve the process ID.
	return static_cast<std::uint32_t>(getpid());
#endif
}


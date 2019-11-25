// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define OSN_STATUS unsigned int
#define OSN_SUCCESS 0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <winsock.h>
#	pragma comment(lib, "WS2_32")
#	define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#endif //PCH_H

#include "pch.h"
#include "net.h"

// Initialise the Origami Sheep Net library
// Returns OSN_SUCCESS (0) on success and a OS Net error code on failure
OSN_STATUS osn_init()
{
#ifdef _WIN32
	WSADATA wsa;
	int code;
	if((code = WSAStartup(MAKEWORD(2, 2), &wsa)) != 0) {
		switch(code) {
		case WSASYSNOTREADY:
			return osn_NOT_READY;
		case WSAVERNOTSUPPORTED:
			return osn_NOT_SUPPORTED;
		case WSAEINPROGRESS:
			return osn_IN_PROGRESS;
		default:
			return osn_INTERNAL;
		}
	}
	return OSN_SUCCESS;
#endif
}

// Uninitialise the Origami Sheep Net library
void osn_uninit()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

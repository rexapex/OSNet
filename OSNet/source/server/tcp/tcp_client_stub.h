#pragma once

struct osnTcpClientStub {
#ifdef _WIN32
	SOCKET socket;
#endif
};

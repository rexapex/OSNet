#include "pch.h"
#include "TcpServer.h"

extern "C" {
#	include "OSNet/source/server/tcp/tcp_server.h"
}

namespace os
{
namespace net
{
	TcpServer::TcpServer(osnServerInfo * info) : handle(osn_tcp_server_new(info))
	{
		if(!handle)
			throw std::runtime_error("Failed to create tcp server");	// TODO - Implement exception wrappers around error codes
	}

	TcpServer::~TcpServer()
	{
		osn_tcp_server_free(handle);
	}
}
}

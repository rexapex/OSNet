#pragma once

struct osnTcpServer;
struct osnServerInfo;

namespace os {
namespace net {
	class TcpServer
	{
	public:
		TcpServer(osnServerInfo * info);
		~TcpServer();
		TcpServer(TcpServer const &) = delete;
		TcpServer & operator=(TcpServer const &) = delete;

		inline operator osnTcpServer * () { return handle; }

	private:
		osnTcpServer * handle;
	};
}
}

#pragma once

enum osnTcpError {
	osn_SOCKET_CREATION_FAILURE = 1,
	osn_ALREADY_LISTENING,
	osn_SOCKET_BIND_FAILURE,
	osn_SOCKET_READ_FAILURE,
	osn_ACCEPT_CLIENT_FAILURE,
	osn_MAX_NUM_CLIENTS_REACHED
};

struct osnServerInfo;
typedef struct osnTcpServer osnTcpServer;

// Create a new TCP server
osnTcpServer * osn_tcp_server_new(struct osnServerInfo * info);

// Free a TCP server
void osn_tcp_server_free(osnTcpServer * s);

// Start the TCP server to begin listening for messages
OSN_STATUS osn_tcp_server_start(osnTcpServer * s);

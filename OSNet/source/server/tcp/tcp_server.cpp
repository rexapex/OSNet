#include "pch.h"
#include "tcp_server.h"
#include "source/internal/util.h"
#include "../server_info.h"
#include "tcp_client_stub.h"

#define MAX_RECV_BUFFER_LEN 1024
#define MAX_NUM_CLIENTS 128

#ifdef _WIN32
struct osnTcpServer {
	SOCKET listen_socket;
	struct sockaddr_in my_addr;
	struct osnServerInfo info;

	struct osnTcpClientStub clients[MAX_NUM_CLIENTS];	// TODO - Maybe make this variable length
	unsigned int num_clients;

	bool listening;
};
#else
#	error "cnTcpServer only supported in Windows builds"
#endif

static OSN_STATUS init(osnTcpServer * s);
static OSN_STATUS mylisten(osnTcpServer * s);

// Create a new TCP server
osnTcpServer * osn_tcp_server_new(struct osnServerInfo * info)
{
	osnTcpServer * s = malloc(sizeof *s);
	if(!s)
		return NULL;
	s->info = *info;
	s->num_clients = 0;
	memset(s->clients, 0, sizeof s->clients);
	if(init(s))
		return s;
	cn_tcp_server_free(s);
	return NULL;
}

// Free a TCP server
void osn_tcp_server_free(osnTcpServer * s)
{
#ifdef _WIN32
	if(s) {
		closesocket(s->listen_socket);	// TODO - Test what happens if socket is never opened
	}
#endif
	free(s);
}

// Start the TCP server to begin listening for messages
OSN_STATUS osn_tcp_server_start(osnTcpServer * s)
{
	if(!s->listening) {
		s->listening = true;
		return mylisten(s);
	}
	return osn_ALREADY_LISTENING;
}

// Initialise a tcp server
// Returns CN_SUCCESS on success and an error code on failure
static OSN_STATUS init(osnTcpServer * s)
{
	s->listening = false;

#ifdef _WIN32
	s->listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s->listen_socket == INVALID_SOCKET) {
		return osn_SOCKET_CREATION_FAILURE;
	}

	s->my_addr.sin_family = AF_INET;
	s->my_addr.sin_port = htonl(s->info.port);
	s->my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif

	return OSN_SUCCESS;
}

// Listen for networked messages
static OSN_STATUS mylisten(osnTcpServer * s)
{
#ifdef _WIN32
	if(bind(s->listen_socket, (struct sockaddr *)&s->my_addr, sizeof s->my_addr) == SOCKET_ERROR) {
		return osn_SOCKET_BIND_FAILURE;
	}

	listen(s->listen_socket, s->info.backlog);

	fd_set readfds;
	const int addrlen = sizeof(struct sockaddr_in);
	byte recv_buffer[MAX_RECV_BUFFER_LEN];

	// Processes messages received from clients
	while(true) {
		FD_ZERO(&readfds);
		FD_SET(s->listen_socket, &readfds);

		for(unsigned i = 0; i < s->num_clients; i++) {
			FD_SET(s->clients[i].socket, &readfds);
		}

		// Wait for activity of any of the sockets
		int activity = select(0, &readfds, NULL, NULL, NULL);
		if(activity == SOCKET_ERROR) {
			return osn_SOCKET_READ_FAILURE;
		}

		// If the activity is on the listen socket, it is an incoming connection
		if(FD_ISSET(s->listen_socket, &readfds)) {
			struct sockaddr_in client;
			SOCKET new_socket = accept(s->listen_socket, (struct sockaddr *)&client, &addrlen);
			if(new_socket == INVALID_SOCKET) {
				return osn_ACCEPT_CLIENT_FAILURE;
			}

			if(s->num_clients >= MAX_NUM_CLIENTS) {
				return osn_MAX_NUM_CLIENTS_REACHED;
			}

			s->clients[s->num_clients].socket = new_socket;
			s->num_clients++;
		} else {
			// Else, check each socket for an IO operation
			for(unsigned i = 0; i < s->num_clients; i++) {
				SOCKET soc = s->clients[i].socket;
				if(FD_ISSET(soc, &readfds)) {
					struct sockaddr_in address;
					getpeername(soc, (struct sockaddr_in *)&address, &addrlen);
				}
			}
		}
	}
#endif
}

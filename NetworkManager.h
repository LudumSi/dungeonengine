
#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <array>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MAX_PACK_BYTES 1024

struct packet {
	uint16_t cmd_id;
	size_t n_args;
	byte* args;
};

class NetworkManager {
protected:
	
	const int port;
	SOCKET listening_sock;
	int listening_sockaddr_len;
	sockaddr_in listening_sockaddr;

	int bind_listener();
	int winsock_init();
	packet* decode_packet(packet*, int);

public:

	NetworkManager(const int);
	~NetworkManager();

};
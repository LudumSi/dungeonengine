
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

enum commands {
	PLAINTEXT,

};

class NetworkManager {
protected:
	
	// Port for sending/receiving information
	const int port;
	SOCKET listening_sock;
	int listening_sockaddr_len;
	sockaddr_in listening_sockaddr;

	int winsock_init();
	//virtual void pk_to_bytes();
	//virtual void bytes_to_pk();

public:

	int bind_listener();
	NetworkManager(const int);
	~NetworkManager();

};
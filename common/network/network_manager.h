
#pragma once 

#include <iostream>
#include <WS2tcpip.h>

//#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MAX_PACK_BYTES 1024
#define MAX_CLIENTS 8

struct packet {
	uint16_t cmd_id;
	size_t n_args;
	byte* args;
};

// Add more command ids here in the future
enum commands {
	CONNECT,
	DISCONNECT,
	MESSAGE
};

int winsock_init();
int bind_listener(sockaddr_in*, SOCKET*, int, const char*);
packet* bytes_to_packet(int*, int);
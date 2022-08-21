
#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <array>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MAX_PACK_BYTES 1024

struct packet {
	
};

class NetworkManager {
private:
	
	const int port;
	const char* ip;
	SOCKET sock;
	sockaddr_in local_sockaddr;
	sockaddr_in other_sockaddr;

	int sock_bind();
	int winsock_init();

public:

	char * recv(bool, sockaddr_in *);
	int send(char *, int, sockaddr_in *);
	int connectTo(char *)

	NetworkManager(const int);
	~NetworkManager();

};



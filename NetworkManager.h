/*
#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <array>

#define MAX_CLIENTS 8

class NetworkManager {
private:
	// Clients and Server Objects should be here
	// Threads should be here/inside client/server objects

	std::array<int, MAX_CLIENTS> clients;

	const int port;
	SOCKET serverSock;
	sockaddr_in serverHint;

	int sock_bind();
	int winsock_init();

public:

	NetworkManager(int);
	~NetworkManager();

};
*/


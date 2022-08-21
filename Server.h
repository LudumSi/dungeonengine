#pragma once
#include "NetworkManager.h"

#include <mutex>
#include <thread>
#include <ctime>


class Server : public NetworkManager {
private:

	sockaddr_in client_sockaddr;

	int in_buf[MAX_PACK_BYTES];
	int out_buf[MAX_PACK_BYTES];
	packet* pack;


public:

	Server();
	~Server();

	int connect(char*);
	int disconnect();
	int send();
	int recv(bool);
};
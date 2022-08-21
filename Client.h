#pragma once
#include "NetworkManager.h"
class Client : public NetworkManager {
private:

	char* ip;
	sockaddr_in server_sockaddr;
	int server_sockaddr_len;

	SOCKET server_sock;

	
	packet* pack;

	
public:
	
	char in_buf[MAX_PACK_BYTES];
	char out_buf[MAX_PACK_BYTES];



	Client(int);
	~Client();

	int connect(const char *);
	int disconnect();
	int send();
	int recv();
};
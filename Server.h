#pragma once
#include "NetworkManager.h"

#include <mutex>
#include <thread>
#include <ctime>
#include <queue>

struct client_data {
	sockaddr_in sockaddr;
	SOCKET sock;
	char in_buf[MAX_PACK_BYTES];
	//std::queue<packet> p_queue;
};

class Server {
private:

	std::thread* c_threads[MAX_CLIENTS];	// client threads
	client_data* c_data[MAX_CLIENTS];		// Client networking/data
	
	std::thread* r_thread;		// listening/receiving thread
	std::thread* game_thread;	// Main game loop

	char in_buf[MAX_PACK_BYTES];
	char out_buf[MAX_PACK_BYTES];
	packet* pack = nullptr;
	int port;
	

public:

	Server(int);
	~Server();

	int open();
	int close();

	int connect(char*);
	int disconnect();
	int send();
	int recv(bool);
};
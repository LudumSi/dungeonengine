
#pragma once 

#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>

#include <entity.h>
#include <component.h>
#include <world.h>

//#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MAX_PACK_BYTES 1024
#define MAX_CLIENTS 8

/* struct packet {
	uint16_t cmd_id;
	size_t n_args;
	byte* args;
};
 */

// Add more command ids here in the future
enum commands {
	CONNECT = 0,
	DISCONNECT,
	MESSAGE,
};

// packet* bytes_to_packet(int*, int);

struct connection {
	sockaddr_in addr;
	SOCKET* sock;

	int addr_len;
	bool open;
	char uid;

	std::mutex outbox_mutex;
	std::queue<std::string> outbox;
	std::thread* tx;

};

int run(connection*);
int update(connection*);
connection * create_connection(sockaddr_in, SOCKET *, const char);


class ConnectionManager {
private:
	std::vector<connection*> connections;
	SOCKET sock;
	sockaddr_in sock_hints;
	sockaddr_in addr;

	const char* ip;
	int port;
	char uid;
	World * world;

	char * serialize_component(Entity, CompID);
	char * serialize_entity(Entity);

	int winsock_init();
	int bind_listener();
	int configure_addr();

public:
	ConnectionManager();
	~ConnectionManager();

	int start(int, const char*, char);
	int run();
	void add_message(std::string, char);
	
};
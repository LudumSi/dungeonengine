
#include "network_manager.h"

connection * create_connection(sockaddr_in addr, SOCKET * sock, const char uid) {
	connection * conn = new connection;

	conn->sock = sock;
	conn->addr = addr;
	conn->addr_len = sizeof(addr);
	conn->uid = uid;

	conn->tx = new std::thread(&update, conn);

	return conn;
}

int run(connection * conn) {
	//conn->tx = new std::thread(&update, conn);
	//conn->tx->join();

	return 0;
}



int update(connection * conn) {
	char buf[MAX_PACK_BYTES];
    // Send out data from outbox
	do {
		ZeroMemory(buf, MAX_PACK_BYTES);
		conn->outbox_mutex.lock();
		if(!conn->outbox.empty()) {
			strcpy(buf, conn->outbox.front().c_str());
            std::cout << "Sending " << buf + 1 << std::endl;

			int bytes_out = sendto(*conn->sock, buf, strlen(buf) + 1, 0, (sockaddr*)&conn->addr, conn->addr_len);
			if(bytes_out == SOCKET_ERROR) {
				std::cout << "SOCKET ERROR in update" << std::endl;
			} else if(bytes_out > 0) {
				conn->outbox.pop();
			} else {
				std::cout << "Something bad happened" << std::endl;
                conn->outbox.pop();
			}
		}
		conn->outbox_mutex.unlock();
	} while(strcmp(buf + 1, "logout")); // Change this later
	return 0;
}

ConnectionManager::ConnectionManager() {
	return;
}

ConnectionManager::~ConnectionManager() {
	return;
}


char * ConnectionManager::serialize_component(Entity entity_id, CompID component_id) {
	return nullptr;
}

void ConnectionManager::add_message(std::string message, char uid) {
	for(auto conn : connections) {
		if(uid == 0 || conn->uid == uid) {
			conn->outbox_mutex.lock();
			conn->outbox.push(this->uid + message);
			conn->outbox_mutex.unlock();
		}
	}
}

int ConnectionManager::start(int port, const char* ip, char uid) {
	this->ip = ip;
	this->port = port;
	this->uid = uid;

	this->winsock_init();
	this->configure_addr();

	if (ip == nullptr) {
		// Server stuff here
		bind_listener();
		connections.reserve(MAX_CLIENTS);
	} else {
		connections.reserve(1); // For client
	}

	if(ip) {
		char buf[MAX_PACK_BYTES];
		buf[0] = uid;
		buf[1] = '\0';
		strcat(buf, "Hello");

		int bytes_out = sendto(sock, buf, strlen(buf) + 1, 0, (sockaddr*)&addr, sizeof(addr));
		connections.push_back(create_connection(addr, &sock, 0));

		if (bytes_out == SOCKET_ERROR) {
			std::cout << "Unable to send connection message" << std::endl;
			return bytes_out;
		}
	}

	return 0;
}

int ConnectionManager::run() {
	char buf[MAX_PACK_BYTES];
	int addr_len = 0;

	ZeroMemory(&addr, addr_len);
	addr_len = sizeof(addr);

	ZeroMemory(&buf, MAX_PACK_BYTES);

	int bytes_in = recvfrom(sock, buf, MAX_PACK_BYTES, 0, (sockaddr*)&addr, &addr_len);

	if(bytes_in == SOCKET_ERROR) {
		std::cout << "Error receiving: " << WSAGetLastError() << std::endl;        
		return SOCKET_ERROR;
	}
	// TODO: Decode more complex packets
	bool new_client = true;
	char ip[256];
	inet_ntop(AF_INET, &addr.sin_addr, ip, 256);

	for(auto conn : connections) {
		if(buf[0] == conn->uid) {
			new_client = false;
			break;
		}
	}

	if(new_client && connections.size() <= MAX_CLIENTS) {
		std::cout << "New client connection from " << ip << " with uid: " << int(buf[0]) << std::endl;
		connections.push_back(create_connection(addr, &sock, buf[0]));
	}

	std::cout << "Received data: " << buf + 1 << std::endl;

	for(auto conn : connections) {
		conn->tx->join();
	}
	return 0;
}

int ConnectionManager::winsock_init() {
	std::cout << "Initializing winsock" << std::endl;

	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOK = WSAStartup(version, &data);

	if (wsOK != 0) {
		std::cout << "Fatal : Can't Start Winsock!" << std::endl;
		return wsOK;
	}
	return 0;
}

int ConnectionManager::configure_addr() {
	std::cout << "Configuring address for " << (ip ? ip : "ANY") << " : " << port << std::endl;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (ip) {
		inet_pton(AF_INET, ip, &addr.sin_addr);
	}
	else {
		addr.sin_addr.S_un.S_addr = ADDR_ANY;
	}
	// Create Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	return 0;
}

int ConnectionManager::bind_listener() {
	// Bind to socket
	int listening_sockaddr_len = sizeof(addr);
	int err =  bind(sock, (sockaddr*)&addr, listening_sockaddr_len);

	// Handle Errors I guess
	if (err == SOCKET_ERROR) {
		std::cout << "Can't bind Socket " << WSAGetLastError() << std::endl;
		return err;
	}
	return err;
}
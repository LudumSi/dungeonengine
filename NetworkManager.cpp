#include "NetworkManager.h"


// Probably should add exceptions here
NetworkManager::NetworkManager(int port = 25565) : port(port) {
	winsock_init();
	sock_bind();


}

NetworkManager::~NetworkManager() {
	winsock_cleanup();

	// Go through and force close all client sockets/threads
	//
}


int NetworkManager::winsock_init() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOK = WSAStartup(version, &data);

	if (wsOK != 0) {
		std::cout << "Fatal : Can't Start Winsock!" << std::endl;
		return wsOK;
	}

	return 0;

}

void winsock_cleanup() {

}

int NetworkManager::sock_bind() {

	serverSock = socket(AF_INET, SOCK_DGRAM, 0);
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(port);

	int err =  bind(serverSock, (sockaddr*)&serverHint, sizeof(serverHint));

	if (err == SOCKET_ERROR) {
		std::cout << "Can't bind Socket " << WSAGetLastError() << std::endl;
		return err;
	}

	return 0;
}

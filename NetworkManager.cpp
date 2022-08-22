
#include "NetworkManager.h"


// Probably should add exceptions here
NetworkManager::NetworkManager(const int port = 25565) : port(port) {

	winsock_init();
}

NetworkManager::~NetworkManager() {
	// Close socket
	std::cout << "Deconstructing NetworkManager" << std::endl;
	closesocket(listening_sock);
	// Shutdown Winsock
	WSACleanup();
}

/*
packet* NetworkManager::decode_packet(packet * packet, int bytesIn) {
	return nullptr;
}
*/

int NetworkManager::winsock_init() {
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

int NetworkManager::bind_listener() {
	std::cout << "Binding listening socket at port " << port << std::endl;

	// Setup Receiving socket local_sockaddrs for UDP and designated port
	listening_sockaddr.sin_family = AF_INET;
	listening_sockaddr.sin_port = htons(port);
	listening_sockaddr.sin_addr.S_un.S_addr = ADDR_ANY;
	listening_sockaddr_len = sizeof(listening_sockaddr);

	// Create Socket
	listening_sock = socket(AF_INET, SOCK_DGRAM, 0);

	// Bind to socket
	int err =  bind(listening_sock, (sockaddr*)&listening_sockaddr, listening_sockaddr_len);

	// Handle Errors I guess
	if (err == SOCKET_ERROR) {
		std::cout << "Can't bind Socket " << WSAGetLastError() << std::endl;
		return err;
	}

	return 0;
}


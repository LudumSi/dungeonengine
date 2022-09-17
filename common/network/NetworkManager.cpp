
#include "NetworkManager.h"

int winsock_init() {
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

int bind_listener(sockaddr_in * listening_sockaddr, SOCKET* listening_sock, int port, const char * ip) {
	std::cout << "Binding listening socket at " << (ip ? ip : "ANY") << " : " << port << std::endl;

	// Setup Receiving socket local_sockaddrs for UDP and designated port
	listening_sockaddr->sin_family = AF_INET;
	listening_sockaddr->sin_port = htons(port);
	
	if (ip) {
		inet_pton(AF_INET, ip, &listening_sockaddr->sin_addr);
	}
	else {
		listening_sockaddr->sin_addr.S_un.S_addr = ADDR_ANY;
	}

	int listening_sockaddr_len = sizeof(*listening_sockaddr);

	// Create Socket
	(*listening_sock) = socket(AF_INET, SOCK_DGRAM, 0);

	// Bind to socket
	int err =  bind(*listening_sock, (sockaddr*)listening_sockaddr, listening_sockaddr_len);

	// Handle Errors I guess
	if (err == SOCKET_ERROR) {
		std::cout << "Can't bind Socket " << WSAGetLastError() << std::endl;
		return err;
	}

	return 0;
}

packet* bytes_to_packet(int* bytes, int n_bytes) {
	
}
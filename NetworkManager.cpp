
#include "NetworkManager.h"


// Probably should add exceptions here
NetworkManager::NetworkManager(const int port = 25565, const char * ip = "") : port(port), ip(ip) {
	winsock_init();
	sock_bind();
}

NetworkManager::~NetworkManager() {
	// Close socket
	closesocket(sock);
	// Shutdown Winsock
	WSACleanup();
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

int NetworkManager::sock_bind() {

	// Server local_sockaddrs for UDP and designated port
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(port);

	// Clients will use this to find the server by IP
	if (strlen(ip) > 0) {
		local_sockaddr.sin_addr.S_un.S_addr = inet_addr(ip);
		// inetpton(AF_INET, ip, &sock.sin_addr);
	}
	else {
		local_sockaddr.sin_addr.S_un.S_addr = ADDR_ANY;
	}

	// Create Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	// Bind to socket
	int err =  bind(sock, (sockaddr*)&local_sockaddr, sizeof(local_sockaddr));

	// Handle Errors I guess
	if (err == SOCKET_ERROR) {
		std::cout << "Can't bind Socket " << WSAGetLastError() << std::endl;
		return err;
	}

	return 0;
}

packet * NetworkManager::recv(bool blocking, sockaddr_in * from) {

	int fromLen = sizeof(from);

	ZeroMemory(from, fromLen);
	ZeroMemory(buf, MAX_PACK_BYTES);

	packet* pack = new packet;
	int bytesIn = 0;
	do {


		recvfrom(sock, buf, MAX_PACK_BYTES, 0, (sockaddr*)from, &fromLen);
		if (bytesIn == SOCKET_ERROR) {
			std::cout << "Error receiving message : " << WSAGetLastError() << std::endl;
			continue; // TODO: Better error handling
		}

	} while(blocking);

	char clientIp[256];
	ZeroMemory(clientIp, 256);

	inet_ntop(AF_INET, &other_sockaddr.sin_addr, clientIp, 256);

	std::cout << "Message recv from " << clientIp << " : " << buf << std::endl;

	return pack;
}

int NetworkManager::send(const char * buf, sockaddr_in* to) {

	int toLen = sizeof(to);

	ZeroMemory(to, toLen);

	int sendOk = sendto(sock, buf, MAX_PACK_BYTES, 0, (sockaddr*)to, toLen);

	if (sendOk == SOCKET_ERROR) {
		std::cout << "Error sending message : " << WSAGetLastError() << std::endl;
		return sendOk; // TODO: Better error handling
	}
	else {
		std::cout << "Sent message to other_sockaddr : " << buf << std::endl;
		return 0;
	}
}
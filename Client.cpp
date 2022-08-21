#include "Client.h"

Client::Client(int port) : NetworkManager(port) {
	std::cout << "Starting Client on port " << port << std::endl;

	ZeroMemory(in_buf, MAX_PACK_BYTES);
	ZeroMemory(out_buf, MAX_PACK_BYTES);

	ip = nullptr;
	server_sockaddr_len = 0;
}

Client::~Client() {
	if (ip) delete[] ip;
}

int Client::connect(const char* ip, int port) {

	if (this->ip) delete[] this->ip;
	this->ip = new char[strlen(ip)];

	std::cout << "Connecting to Server at " << ip << ":" << port << std::endl;

	// Setup Receiving socket local_sockaddrs for UDP and designated port
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &server_sockaddr.sin_addr);
	server_sockaddr_len = sizeof(server_sockaddr);

	// Create Socket
	server_sock = socket(AF_INET, SOCK_DGRAM, 0);

	return 0;
}

int Client::disconnect() {
	// Nothing for now
	closesocket(server_sock);

	return 0;
}

int Client::recv() {
	std::cout << "Waiting for message on port " << port << std::endl;

	ZeroMemory(in_buf, MAX_PACK_BYTES);

	packet* pack = new packet;
	int bytesIn = 0;

	bytesIn = recvfrom(listening_sock, in_buf, MAX_PACK_BYTES, 0, (sockaddr*)&listening_sockaddr, &listening_sockaddr_len);
	if (bytesIn == SOCKET_ERROR) {
		std::cout << "Error receiving message : " << WSAGetLastError() << std::endl;
		return -1; // TODO: Better error handling
	}

	std::cout << "Message recv: " << in_buf << std::endl;

	return bytesIn;
}

int Client::send() {
	std::cout << "Sending message to server : " << out_buf << std::endl;

	int sendOk = sendto(server_sock, out_buf, MAX_PACK_BYTES, 0, (sockaddr*)&server_sockaddr, server_sockaddr_len);

	if (sendOk == SOCKET_ERROR) {
		std::cout << "Error sending message : " << WSAGetLastError() << std::endl;
		return sendOk; // TODO: Better error handling
	}
	else {
		std::cout << "Sent message to other_sockaddr : " << in_buf << std::endl;
		return 0;
	}
}
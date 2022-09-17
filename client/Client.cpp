#include "Client.h"

Client::Client(int port) {
	winsock_init();

	std::cout << "Starting Client on port " << port << std::endl;

	ZeroMemory(in_buf, MAX_PACK_BYTES);
	ZeroMemory(out_buf, MAX_PACK_BYTES);

	ip = nullptr;
}

Client::~Client() {
	if (ip) delete[] ip;
}

int Client::connect(const char* ip) {

	if (this->ip) delete[] this->ip;
	this->ip = new char[strlen(ip)];

	std::cout << "Connecting to Server at " << ip << ":" << port << std::endl;

	// Setup Receiving socket local_sockaddrs for UDP and designated port
	listening_sockaddr.sin_family = AF_INET;
	listening_sockaddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &listening_sockaddr.sin_addr);
	listening_sockaddr_len = sizeof(listening_sockaddr);


	return 0;
}

int Client::disconnect() {
	// Nothing for now
	closesocket(listening_sock);

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
	//std::cout << "Sending message to server : " << out_buf << std::endl;

	int sendOk = sendto(listening_sock, out_buf, MAX_PACK_BYTES, 0, (sockaddr*)&listening_sockaddr, listening_sockaddr_len);

	if (sendOk == SOCKET_ERROR) {
		std::cout << "Error sending message : " << WSAGetLastError() << std::endl;
		return sendOk; // TODO: Better error handling
	}
	else {
		//std::cout << "Sent message to other_sockaddr : " << out_buf << std::endl;
		return 0;
	}
}
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <cstring>

#include "network/network_manager.h"

//std::mutex outbox_mutex;
//std::mutex inbox_mutex;

//std::queue<std::string> outbox;
//std::queue<std::string> inbox;

//std::vector<sockaddr_in*> client_list;
/* 
void process_inbox() {
    std::string message;
    do {
        message = "";
        
        inbox_mutex.lock();
        
        if(!inbox.empty()) {
            message = inbox.front();
            inbox.pop();
            std::cout << message << std::endl;
        }
                
        inbox_mutex.unlock();
        
    } while(message != "logout");
}

void process_outbox() {
    std::string message;
    do {
        printf("Input CLIENT message: ");
        getline(std::cin, message, '\n');
        
        outbox_mutex.lock();
        outbox.push(message);
        outbox_mutex.unlock();

    } while(message != "logout");

}

void recv_clients(SOCKET* server_socket, sockaddr_in* client) {
    std::string message;
	char buf[1024];
    int client_length = 0;
	do {
		// Wait for messages from clients
		client_length = sizeof(*client);
		ZeroMemory(client, client_length);
		
		ZeroMemory(buf, 1024);

		int bytes_in = recvfrom(*server_socket, buf, 1024, 0, (sockaddr*)client, &client_length);

		if(bytes_in == SOCKET_ERROR) {
            std::cout << "Error receiving from client" << WSAGetLastError() << std::endl;        
            continue;
		}

		// WE SHOULD GET INFORMATION FROM THE CLIENT HERE
		// can verify things like IP and specific ids
		// Turn data into packeted info?

		if(bytes_in > 0) {
			message = buf;
			std::cout << "Received data from someone" << std::endl;
			inbox_mutex.lock();
			inbox.push(buf);
			inbox_mutex.unlock();
		}
	} while(message != "logout");
}

void send_clients(SOCKET* server_socket, sockaddr_in* client) {
    //ZeroMemory(&client, client_length);
    std::string message;
    // Serialize data here?

    // Send out data from outbox
	do {
		outbox_mutex.lock();
		if(!outbox.empty()) {
			message = outbox.front();
            std::cout << "Sending " << message << " to someone" << std::endl;

			int bytes_out = sendto(*server_socket, message.c_str(), message.length() + 1, 0, (sockaddr*)client, sizeof(client));
			if(bytes_out > 0) {
				outbox.pop();
			} else {
				std::cout << "UHHHHHHHH" << std::endl;
                outbox.pop();

			}
		}
		outbox_mutex.unlock();
	} while(message != "logout");
    
}
 */
int main() {

    std::cout << "Starting Server" << std::endl;

    // Init Network Here
    SOCKET server_socket;
    sockaddr_in server_hints;
    sockaddr_in client;
    int port = 7777;
    const char * ip = "192.168.254.11";

    ConnectionManager c;
    c.start(port, nullptr, 0);
    c.run();

    /* 
    if(winsock_init() > 0) {
        return EXIT_FAILURE;
    }

    if(bind_listener(&server_hints, &server_socket, port, ip) > 0) {
        return EXIT_FAILURE;
    }

    // Open two threads
    std::thread network_listener(recv_clients, &server_socket, &client);
    //std::thread network_poster(send_clients, &server_socket, &client);
    std::thread inboxer(process_inbox); // Processes incoming data
    std::thread outboxer(process_outbox); // Processes outgoing data. This belongs in the client

    network_listener.join();
    //network_poster.join();

    inboxer.join();
    outboxer.join();

    WSACleanup();
    closesocket(server_socket);

    std::cout << "Complete" << std::endl;
    return 0; 
    */
}
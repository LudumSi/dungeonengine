#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <cstring>

#include "network/network_manager.h"

std::mutex outbox_mutex;
std::mutex inbox_mutex;

std::queue<std::string> outbox;
std::queue<std::string> inbox;

std::vector<sockaddr_in*> client_list;

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
        std::cin >> message;
        
        outbox_mutex.lock();
        outbox.push(message);
        outbox_mutex.unlock();

    } while(message != "logout");

}

void recv_clients(SOCKET* server_socket, sockaddr_in* client) {
    // Wait for messages from clients
    int client_length = sizeof(client);
    ZeroMemory(&client, client_length);
    
    char buf[1024];
    ZeroMemory(buf, 1024);

    int bytes_in = recvfrom(*server_socket, buf, 1024, 0, (sockaddr*)client, &client_length);

    if(bytes_in == SOCKET_ERROR) {
        // Error bad
    }

    // WE SHOULD GET INFORMATION FROM THE CLIENT HERE
    // can verify things like IP and specific ids
    // Turn data into packeted info?

    if(bytes_in > 0) {
        inbox_mutex.lock();
        inbox.push(buf);
        inbox_mutex.unlock();
    }
    
    
}

void send_clients(SOCKET* server_socket, sockaddr_in* client) {
    //ZeroMemory(&client, client_length);
    
    // Serialize data here?

    // Send out data from outbox
    outbox_mutex.lock();
    while(outbox.empty() > 0) {
        int bytes_out = sendto(*server_socket, outbox.front().c_str(), outbox.front().length() + 1, 0, (sockaddr*)client, sizeof(client));
        if(bytes_out > 0) {
            outbox.pop();
        }
    }
    outbox_mutex.unlock();
}

int main() {

    std::cout << "Starting Server" << std::endl;

    // Init Network Here
    SOCKET server_socket;
    sockaddr_in server_hints;
    sockaddr_in client;
    int port = 7777;
    winsock_init();
    if(bind_listener(&server_hints, &server_socket, port, nullptr) > 0) {
        return EXIT_FAILURE;
    }

    // Open two threads
    std::thread network_listener(recv_clients, &server_socket, &client);
    std::thread network_poster(send_clients, &server_socket, &client);
    std::thread inboxer(process_inbox); // Processes incoming data
    std::thread outboxer(process_outbox); // Processes outgoing data. This belongs in the client

    network_listener.join();
    network_poster.join();
    
    inboxer.join();
    outboxer.join();

    std::cout << "Complete" << std::endl;
    return 0;
}
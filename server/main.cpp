#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "network/network_manager.h"

int main() {

    std::cout << "Starting Server" << std::endl;

    // Init Network Here
    SOCKET server_socket;
    sockaddr_in server_hints;
    sockaddr_in client;
    int port = 7777;
    const char * ip = "127.0.0.1";

    ConnectionManager c;
    c.start(port, nullptr, 0);
    while(1) {
        c.run();
        sleep(1);
    }

}
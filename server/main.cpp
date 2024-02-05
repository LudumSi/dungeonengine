#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <network/tcp_connection_manager.h>

int main() {

    std::cout << "Starting Server" << std::endl;

    // Init Network Here
    int port = 7777;

    TCPServerManager network_manager(port);
    while(1) {
        network_manager.update();
        sleep(1);
    }

}
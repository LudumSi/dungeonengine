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

    try {
        std::cout << "Initializing Network Manager" << std::endl;
        TCPServerManager network_manager(port);
        std::cout << "Initialized Network Manager" << std::endl;
        while(1) {
            std::cout << "Updating Network Manager" << std::endl;
            try {
                network_manager.update();
            }
            catch(int e) {
                std::cout << "An exception occured" << std::endl;
            }
            catch (...) {
                std::cout << "Something went wrong" << std::endl;
            }
        }
    }
    catch(int e) {
        std::cout << "An exception occured" << std::endl;
    }
    catch (...) {
        std::cout << "Something went wrong" << std::endl;
    }
    std::cout << "Exiting" << std::endl;
}
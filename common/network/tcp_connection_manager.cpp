
#pragma once 

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#include <network/tcp_connection_manager.h>

#pragma comment (lib, "Ws2_32.lib")

TCPClientManager::TCPClientManager(const char* ip, int port) {
    return;
}

int TCPClientManager::connect() {
    return 0;
}

int TCPClientManager::disconnect() {
    return 0;
}

void TCPClientManager::update() {
    return;
}

TCPServerManager::TCPServerManager(int port) {
    return;
}

void TCPServerManager::update() {
    return;
}
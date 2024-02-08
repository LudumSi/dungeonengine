#pragma once 

#include <iostream>

#include <network/tcp_connection_manager.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512

TCPClientManager::TCPClientManager() {
    // Initialize Winsock
    int iResult;
    WSADATA wsaData;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        throw iResult;
    }
}

TCPClientManager::~TCPClientManager() {
    disconnect_from_server();
    WSACleanup();
}

void TCPClientManager::set_server_info(const char* ip, int port) {
    this->ip = ip;
    this->port = port;
}

int TCPClientManager::connect_to_server() {

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    char port_str[6]; // max port length is 5 characters + null termination
    itoa(this->port, port_str, 10);
    iResult = getaddrinfo(this->ip, port_str, &hints, &result);
    if ( iResult != 0 ) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        throw iResult;
    }

    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        this->sock = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (this->sock == INVALID_SOCKET) {
            int err = WSAGetLastError();
            std::cout << "socket failed with error: " << err << std::endl;
            throw err;
        }

        // Connect to server.
        iResult = connect(this->sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (this->sock == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        return 1;
    }

    std::cout << "Connected to server at " << this->ip << ":" << this->port << std::endl;
    return 0;
}

int TCPClientManager::disconnect_from_server() {

    int iResult;

    if(this->sock != INVALID_SOCKET) {
        // shutdown the connection since no more data will be sent
        iResult = shutdown(this->sock, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cout << "shutdown failed with error: " << err << std::endl;
            return err;
        }
        closesocket(this->sock);
    }

    this->sock = INVALID_SOCKET;
    return 0;
}

void TCPClientManager::update() {

    if (this->sock == INVALID_SOCKET) {
        this->connect_to_server();
        return;
    }

    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[DEFAULT_BUFLEN] = "Do you have the time?\0";
    int recvbuflen = DEFAULT_BUFLEN;

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    TIMEVAL t;
    t.tv_usec = 10000;

    // Used to create lists of sockets to select (check for read/write availabiliy). 
    // Storing as separate sets because in the future we may only want to send or receive on a given update.
    FD_SET(this->sock, &read_set);
    FD_SET(this->sock, &write_set);

    iResult = select(0, nullptr, &write_set, nullptr, &t);
    if (iResult == SOCKET_ERROR) {
        int err = WSAGetLastError();
        std::cout << "send failed with error: " << err << std::endl;
        throw err;
    } else if (iResult == 0) {
        std::cout << "Unable to send data..." << std::endl;
    } else if (iResult > 0) {
        iResult = send(this->sock, sendbuf, (int) strlen(sendbuf), 0 );
        if (iResult == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cout << "send failed with error: " << err << std::endl;
            throw err;
        }
        std::cout << "Asking for the time. " << iResult << " bytes Sent..." << std::endl;
    } else {
        std::cout << "Something unexpected happened" << std::endl;
    }

    // Receive until the peer closes the connection
    // Will need to change this to read until all data has been read
    iResult = select(0, &read_set, nullptr, nullptr, &t);
    if (iResult == SOCKET_ERROR) {
        int err = WSAGetLastError();
        std::cout << "send failed with error: " << err << std::endl;
        throw err;
    } else if (iResult == 0) {
        std::cout << "No data available to read..." << std::endl;
    } else if (iResult > 0) {
        iResult = recv(this->sock, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            std::cout << iResult << " bytes received: " << std::string(recvbuf) << std::endl;
        else if ( iResult == 0 )
            std::cout << "Connection closed\n" << std::endl;
        else if (iResult == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
            throw err;
        }
    } else {
        std::cout << "Something unexpected happened" << std::endl;
    }
}

TCPServerManager::TCPServerManager(int port) {

    this->port = port;

    WSADATA wsaData;
    int iResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        throw iResult;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    char port_str[6]; // max port length is 5 characters + null termination
    itoa(this->port, port_str, 10);
    iResult = getaddrinfo(NULL, port_str, &hints, &result);
    if ( iResult != 0 ) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        throw iResult;
    }

    // Create a SOCKET for the server to listen for client connections.
    this->client_listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->client_listener == INVALID_SOCKET) {
        int err = WSAGetLastError();
        std::cout << "socket failed with error: " << err << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        throw err;
    }

    // Configuring listener socket as non-blocking so that the system 
    // can check new connections without necessitating a separate thread.
    unsigned long iMode = 1;
    iResult = ioctlsocket(this->client_listener, FIONBIO, (unsigned long *) &iMode);
    if (this->client_listener == INVALID_SOCKET) {
        int err = WSAGetLastError();
        std::cout << "socket failed with error: " << err << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        throw err;
    }

    // Setup the TCP listening socket
    iResult = bind(this->client_listener, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        int err = WSAGetLastError();
        std::cout << "bind failed with error: " << err << std::endl;
        freeaddrinfo(result);
        closesocket(this->client_listener);
        WSACleanup();
        throw err;
    }

    freeaddrinfo(result);

    // Everything past this point should be broken out to a separate listener thread in the future
    iResult = listen(this->client_listener, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        int err = WSAGetLastError();
        std::cout << "listen failed with error: " << err << std::endl;
        closesocket(this->client_listener);
        WSACleanup();
        throw err;
    }

    std::cout << "Initialized server on port " << this->port << std::endl;
}

void TCPServerManager::check_connections() {

    FD_ZERO(&read_set);
    FD_SET(this->client_listener, &read_set);
    TIMEVAL t;
    t.tv_usec = 1000;
    t.tv_sec = 0;

    int iResult;

    //std::cout << "Checking for new connections" << std::endl;

    iResult = select(0, &read_set, nullptr, nullptr, &t);
    if (iResult == SOCKET_ERROR) {
        int err = WSAGetLastError();
        std::cout << "listener check failed with error: " << err << std::endl;
        throw err;
    } else if (iResult == 0) {
        // std::cout << "no new clients..." << std::endl;
    } else if (iResult > 0) {
        for (u_int i = 0; i < iResult; i++) {
            // Accept a client socket
            std::cout << "Found valid socket. Connecting..." << std::endl;
            SOCKET sock = accept(this->client_listener, NULL, NULL);
            //SOCKADDR sock_addr;
            //int sock_size;
            //getsockname(sock, &sock_addr, &sock_size);
            if (sock == INVALID_SOCKET) {
                int err = WSAGetLastError();
                std::cout << "accept failed with error: " << err << std::endl;
                throw err;
            } else {
                std::cout << "new client connected" << std::endl;
                this->socks.push_back(sock);
            }
        }
    }
}

void TCPServerManager::update() {
    // Update loop starts by checking for any potential info from the client, 
    // then sends out all info in the outbox.
    this->check_connections();

    // Placeholder. For now we are just sending pings.
    int iResult;
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    char sendbuf[DEFAULT_BUFLEN] = "\0";
    std::string message;
    TIMEVAL t;
    t.tv_usec = 1000;

    time_t now = time(0);
    std::string dt = ctime(&now);
    
    message = "Num Clients: " + std::to_string(this->socks.size()) + ", Time: " + dt;
    strcat(sendbuf, message.c_str());

    // Doing this one socket at a time because we need to be able to clear socks when disconnections occur
    for (u_int i = 0; i < this->socks.size(); i++) {
        FD_ZERO(&read_set);
        FD_SET(this->socks[i], &read_set);

        iResult = select(0, &read_set, nullptr, nullptr, &t);
        if (iResult == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cout << "recv failed with error: " << err << std::endl;
            continue;
        } else if (iResult == 0) {
            std::cout << "No data available to read..." << std::endl;
        } else if (iResult > 0) {
            iResult = recv(this->socks[i], recvbuf, recvbuflen, 0);
            if (iResult > 0)
                std::cout << iResult << " bytes received: " << std::string(recvbuf) << std::endl;
            else if (iResult == 0) {
                std::cout << "client connection closed\n" << std::endl;

                closesocket(this->socks[i]);
                this->socks.erase(std::next(this->socks.begin(), i--));
            } else if (iResult == SOCKET_ERROR) {
                int err = WSAGetLastError();
                std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
                continue;
            }
        }
    }

    for (u_int i = 0; i < this->socks.size(); i++) {
        FD_ZERO(&write_set);
        FD_SET(this->socks[i], &write_set);

        iResult = select(0, nullptr, &write_set, nullptr, &t);
        if (iResult == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cout << "send failed with error: " << err << std::endl;
            continue;
        } else if (iResult == 0) {
            std::cout << "Unable to send data..." << std::endl;
        } else if (iResult > 0) {
            iResult = send(this->socks[i], sendbuf, (int) strlen(sendbuf), 0 );
            if (iResult == SOCKET_ERROR) {
                int err = WSAGetLastError();
                std::cout << "send failed with error: " << err << std::endl;
                continue;
            }
            std::cout << "Asking for the time. " << iResult << " bytes Sent..." << std::endl;
        } else {
            std::cout << "Something unexpected happened when selecting writefds" << std::endl;
        }
    }
}

TCPServerManager::~TCPServerManager() {
    closesocket(this->client_listener);
    for (SOCKET sock: this->socks) {
        closesocket(sock);
    }
    this->socks.clear();
    WSACleanup();
}
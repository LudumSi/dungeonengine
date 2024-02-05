#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//#include <entity.h>
//#include <component.h>
//#include <world.h>
//#include <network/fbs/entity_generated.h>

#pragma comment (lib, "Ws2_32.lib")


class TCPClientManager {
private:
    SOCKET sock = INVALID_SOCKET;
    FD_SET read_set;
    FD_SET write_set;

    const char* ip = nullptr;
    int port;



public:
    // Connect and disconnect implemented as separate functions from constructor/destructor to allow changing servers in the future.
    TCPClientManager(void);
    ~TCPClientManager(void);
    int connect_to_server(const char*, int); 
    int disconnect_from_server(void);
    void update(void);
};

class TCPServerManager {
private:
    SOCKET client_listener = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;
    FD_SET read_set;
    FD_SET write_set;
    
    int port;

public:
    TCPServerManager(int);
    ~TCPServerManager(void);

    int init(void);
    void update(void);
};
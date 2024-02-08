#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>

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
    TCPClientManager();
    ~TCPClientManager();
    void set_server_info(const char*, int);
    int connect_to_server();
    int disconnect_from_server(void);
    void update(void);
};

class TCPServerManager {
private:
    SOCKET client_listener = INVALID_SOCKET;
    std::vector<SOCKET> socks;
    //SOCKET client_socket = INVALID_SOCKET;
    FD_SET read_set; // not to be confused with the macro FD_SET
    FD_SET write_set;
    
    int port;

    void check_connections(void);

public:
    TCPServerManager(int);
    ~TCPServerManager();

    int init(void);
    void update(void);

};
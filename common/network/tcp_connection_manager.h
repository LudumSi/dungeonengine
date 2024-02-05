#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

//#include <entity.h>
//#include <component.h>
//#include <world.h>
//#include <network/fbs/entity_generated.h>

#pragma comment (lib, "Ws2_32.lib")


class TCPClientManager {
private:
    SOCKET sock = INVALID_SOCKET;

public:
    TCPClientManager(const char*, int);
    int connect(void);
    int disconnect(void);
    void update(void);
};

class TCPServerManager {
private:
    SOCKET rx_sock = INVALID_SOCKET;
    SOCKET tx_sock = INVALID_SOCKET;

public:
    TCPServerManager(int);
    void update(void);
};
#ifndef LASER_SCAN_CLIENT_H
#define LASER_SCAN_CLIENT_H

#include <vector>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>



//#pragma comment(lib, "ws2_32.lib")

class LaserScanClient {
public:
    // Konstruktor und Destruktor
    LaserScanClient(const std::string& ip, int port);
    ~LaserScanClient();

    // Methoden
    bool connectToServer();
    std::string readLaserData();

private:
    // Private Methoden
    void initializeWinsock();
    void createSocket();
    void setupServerAddress();
    void cleanup();

    // Member-Variablen
    std::string ip_address;
    int port;
    SOCKET sock;
    struct sockaddr_in server_addr;
};

#endif // LASER_SCAN_CLIENT_H

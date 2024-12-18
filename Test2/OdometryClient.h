#ifndef ODOMETRY_CLIENT_H
#define ODOMETRY_CLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

//#pragma comment(lib, "ws2_32.lib")

class OdometryClient {
public:
    // Konstruktor und Destruktor
    OdometryClient(const std::string& ip, int port);
    ~OdometryClient();

    // Methoden
    bool connectToServer();
    std::string readOdometryData();

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

#endif // ODOMETRY_CLIENT_H

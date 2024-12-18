#ifndef GEOMETRY_MSGS_CLIENT_H
#define GEOMETRY_MSGS_CLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

//#pragma comment(lib, "ws2_32.lib")

class GeometryMsgsClient {
public:
    // Konstruktor und Destruktor
    GeometryMsgsClient(const std::string& ip, int port);
    ~GeometryMsgsClient();

    // Methoden
    bool connectToServer();
    bool sendGeometryMsg(double linear, double angular);

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

#endif // GEOMETRY_MSGS_CLIENT_H

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

//#pragma comment(lib, "ws2_32.lib")

class Server {
public:
    // Konstruktor und Destruktor
    Server(unsigned short port);
    ~Server();

    // Methoden
    void start();

private:
    // Private Methoden
    void initializeWinsock();
    void createSocket();
    void bindSocket();
    void listenForConnections();
    void handleClient(SOCKET clientSocket);
    void cleanup();

    // Fehlerbehandlungsfunktion
    void dieWithError(const std::string &errorMessage);

    // Member-Variablen
    unsigned short server_port;
    SOCKET server_socket;
    struct sockaddr_in server_address;
};

#endif // SERVER_H

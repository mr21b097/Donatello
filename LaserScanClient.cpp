#include <iostream>
#include <winsock2.h>               // Header-Datei für Sockets
#include <ws2tcpip.h>               // Für die IP-Adresse und andere Netzwerkfunktionen
#pragma comment(lib, "ws2_32.lib")  // Linke mit ws2_32.lib

class LaserScanClient {
public:
    LaserScanClient(const std::string& ip, int port) {
        // Winsock initialisieren
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Winsock initialization failed" << std::endl;
            return;
        }

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    ~LaserScanClient() {
        closesocket(sock);  // Socket schließen
        WSACleanup();  // Winsock aufräumen
    }

    void connectToServer() {
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            return;
        }
    }

    void readLaserData() {
        char buffer[1024];
        while (true) {
            int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
                // Daten verarbeiten und in gemeinsamen Speicher schreiben
                std::cout << "Received laser scan data: " << buffer << std::endl;
            }
        }
    }

private:
    SOCKET sock;
    struct sockaddr_in server_addr;
};

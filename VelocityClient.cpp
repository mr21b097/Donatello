#include <iostream>
#include <winsock2.h>  // Windows-kompatible Header-Datei für Sockets
#include <ws2tcpip.h>  // Für die IP-Adresse und andere Netzwerkfunktionen
#pragma comment(lib, "ws2_32.lib")  // Linke mit ws2_32.lib
#include <unistd.h>

class VelocityClient {
public:
    VelocityClient(const std::string& ip, int port) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    ~VelocityClient() {
        close(sock);
    }

    void connectToServer() {
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            return;
        }
    }

    void sendVelocityCommand(float linear, float angular) {
        std::string command = "linear: " + std::to_string(linear) + " angular: " + std::to_string(angular);
        send(sock, command.c_str(), command.length(), 0);
        std::cout << "Sent velocity command: " << command << std::endl;
    }

private:
    int sock;
    struct sockaddr_in server_addr;
};

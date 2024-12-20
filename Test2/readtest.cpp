#include <iostream>
#include <string>
#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

void initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed" << std::endl;
        throw std::runtime_error("Winsock initialization failed");
    }
}

SOCKET createSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        throw std::runtime_error("Socket creation failed");
    }
    return sock;
}

void setupServerAddress(struct sockaddr_in& server_addr, const std::string& ip, int port) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address format" << std::endl;
        throw std::runtime_error("Invalid IP address format");
    }
}

bool connectToServer(SOCKET sock, const struct sockaddr_in& server_addr) {
    if (connect(sock, reinterpret_cast<const struct sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Connected to server!" << std::endl;
    return true;
}

std::string readLaserData(SOCKET sock) {
    char buffer[1024];
    std::string data;
    while (true) {
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            data.append(buffer, bytes_received);
        } else if (bytes_received == 0) {
            std::cout << "Connection closed by server" << std::endl;
            break;
        } else {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            break;
        }
    }
    return data;
}

void cleanup(SOCKET sock) {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    WSACleanup();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    int port = std::stoi(argv[2]);

    try {
        initializeWinsock();

        SOCKET sock = createSocket();

        struct sockaddr_in server_addr;
        setupServerAddress(server_addr, server_ip, port);

        if (!connectToServer(sock, server_addr)) {
            cleanup(sock);
            return 1;
        }

        std::string data = readLaserData(sock);
        std::cout << "Laser scan data received: " << data << std::endl;

        cleanup(sock);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

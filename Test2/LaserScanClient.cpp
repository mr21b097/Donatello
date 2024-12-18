#include "LaserScanClient.h"
#include <iostream>

// Konstruktor
LaserScanClient::LaserScanClient(const std::string& ip, int port)
    : ip_address(ip), port(port), sock(INVALID_SOCKET) {
    initializeWinsock();
    createSocket();
    setupServerAddress();
}

// Destruktor
LaserScanClient::~LaserScanClient() {
    cleanup();
}

// Initialisierung von Winsock
void LaserScanClient::initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed" << std::endl;
        throw std::runtime_error("Winsock initialization failed");
    }
}

// Socket erstellen
void LaserScanClient::createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        throw std::runtime_error("Socket creation failed");
    }
}

// Serveradresse konfigurieren
void LaserScanClient::setupServerAddress() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr);
}

// Verbindung zum Server herstellen
bool LaserScanClient::connectToServer() {
    if (connect(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Connected to server!" << std::endl;
    return true;
}

// Laserdaten lesen
std::string LaserScanClient::readLaserData() {
    char buffer[10024];
    std::string data;
    while (true) {
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-Terminierung für die Ausgabe
            data.append(buffer, bytes_received); // Daten anhängen
            //std::cout << "Received laser scan data: " << buffer << std::endl;
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

// Ressourcen freigeben
void LaserScanClient::cleanup() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    WSACleanup();
}

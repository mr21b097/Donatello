#include "GeometryMsgsClient.h"
#include <iostream>
#include <sstream>

// Konstruktor
GeometryMsgsClient::GeometryMsgsClient(const std::string& ip, int port)
    : ip_address(ip), port(port), sock(INVALID_SOCKET) {
    initializeWinsock();
    createSocket();
    setupServerAddress();
}

// Destruktor
GeometryMsgsClient::~GeometryMsgsClient() {
    cleanup();
}

// Initialisierung von Winsock
void GeometryMsgsClient::initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed" << std::endl;
        throw std::runtime_error("Winsock initialization failed");
    }
}

// Socket erstellen
void GeometryMsgsClient::createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        throw std::runtime_error("Socket creation failed");
    }
}

// Serveradresse konfigurieren
void GeometryMsgsClient::setupServerAddress() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr);
}

// Verbindung zum Server herstellen
bool GeometryMsgsClient::connectToServer() {
    if (connect(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Connected to server!" << std::endl;
    return true;
}

// geometry_msgs senden
bool GeometryMsgsClient::sendGeometryMsg(double linear, double angular) {
    // Erstellen der Nachricht im gewünschten Format
    std::ostringstream message;
    message << "---START---{\"geometry_msgs\": {\"linear\": " << linear << ", \"angular\": " << angular << "}}___END___";
    std::string command = message.str();

    // Nachricht senden
    int bytes_sent = send(sock, command.c_str(), command.size(), 0);
    if (bytes_sent == SOCKET_ERROR) {
        std::cerr << "Failed to send message: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Sent message: " << command << std::endl;
    return true;
}

// Ressourcen freigeben
void GeometryMsgsClient::cleanup() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    WSACleanup();
}

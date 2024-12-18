#include "Server.h"
#include <iostream>
#include <sstream>
#include <cstring>

// Konstruktor
Server::Server(unsigned short port)
    : server_port(port), server_socket(INVALID_SOCKET) {
    initializeWinsock();
    createSocket();
    bindSocket();
}

// Destruktor
Server::~Server() {
    cleanup();
}

// Initialisierung von Winsock
void Server::initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        dieWithError("WSAStartup() failed");
    }
}

// Socket erstellen
void Server::createSocket() {
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        dieWithError("socket() failed");
    }
}

// Socket binden
void Server::bindSocket() {
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        dieWithError("bind() failed");
    }
}

// Auf Verbindungen warten
void Server::listenForConnections() {
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        dieWithError("listen() failed");
    }
    std::cout << "Server is listening on port " << server_port << std::endl;
}

// Client-Verbindung bearbeiten
void Server::handleClient(SOCKET clientSocket) {
    std::string response = "---START---{\"message\": \"Hello, Client!\"}___END___";
    send(clientSocket, response.c_str(), response.size(), 0);
    closesocket(clientSocket);
}

// Fehlerbehandlungsfunktion
void Server::dieWithError(const std::string &errorMessage) {
    std::cerr << errorMessage << std::endl;
    cleanup();
    std::exit(1);
}

// Ressourcen freigeben
void Server::cleanup() {
    if (server_socket != INVALID_SOCKET) {
        closesocket(server_socket);
    }
    WSACleanup();
}

// Server starten
void Server::start() {
    listenForConnections();

    for (;;) {
        sockaddr_in client_address;
        int client_length = sizeof(client_address);

        SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
        if (client_socket == INVALID_SOCKET) {
            dieWithError("accept() failed");
        }

        std::cout << "Handling client " << inet_ntoa(client_address.sin_addr) << std::endl;

        handleClient(client_socket);
    }
}

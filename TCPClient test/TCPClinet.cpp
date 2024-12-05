#include "TCPClinet.h"

#include <iostream>
#include <ws2tcpip.h> // Für inet_pton und andere Netzwerkfunktionen

#pragma comment(lib, "Ws2_32.lib") // Link zur Winsock-Bibliothek

TCPClient::TCPClient(std::string ip) : connected(false), ipAddress(ip) {
    WSADATA wsaData;
    int result;

    // Initialisierung der WinSock-Bibliothek
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup fehlgeschlagen: " << result << "\n";
        return;
    }

    // Erstelle einen Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cout << "Fehler beim Erstellen des Sockets: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Server-Adresse konfigurieren
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(defaultPort); // Standardport
    int addrResult = inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);
    if (addrResult <= 0) {
        std::cout << "Ungültige IP-Adresse.\n";
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    // Verbindung zum Server herstellen
    int connResult = connect(sockfd, (sockaddr*)&server, sizeof(server));
    if (connResult == SOCKET_ERROR) {
        std::cout << "Verbindung fehlgeschlagen: " << WSAGetLastError() << "\n";
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    connected = true;
    std::cout << "Verbindung zu " << ipAddress << " auf Port " << defaultPort << " hergestellt.\n";
}

void TCPClient::receiveData() {
    if (!connected) {
        std::cout << "Keine Verbindung vorhanden.\n";
        return;
    }

    char buffer[1024]; // Puffer für empfangene Daten

    // Empfangene Daten lesen
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminieren
        std::cout << "Empfangene Daten: " << buffer << "\n";
    } else if (bytesReceived == 0) {
        std::cout << "Server hat die Verbindung geschlossen.\n";
    } else {
        std::cout << "Fehler beim Empfang der Daten: " << WSAGetLastError() << "\n";
    }
}

TCPClient::~TCPClient() {
    if (connected) {
        closesocket(sockfd);
        WSACleanup();
        std::cout << "Verbindung geschlossen.\n";
    }
}

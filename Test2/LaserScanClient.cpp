#include "LaserScanClient.h"

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
        std::cerr << "Winsock-Initialisierung fehlgeschlagen" << std::endl;
        throw std::runtime_error("Winsock-Initialisierung fehlgeschlagen");
    }
}

// Socket erstellen
void LaserScanClient::createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket-Erstellung fehlgeschlagen: " << WSAGetLastError() << std::endl;
        throw std::runtime_error("Socket-Erstellung fehlgeschlagen");
    }
}

// Serveradresse konfigurieren
void LaserScanClient::setupServerAddress() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Ungültige IP-Adresse: " << ip_address << std::endl;
        throw std::runtime_error("Ungültige IP-Adresse");
    }
}

// Verbindung zum Server herstellen (mit Wiederholungsversuchen)
bool LaserScanClient::connectToServer() {
    while (true) {
        std::cout << "Versuche, Verbindung zum Server herzustellen (IP: " 
                  << ip_address << ", Port: " << port << ") ..." << std::endl;
        if (connect(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Verbindung fehlgeschlagen: " << WSAGetLastError() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Warte 2 Sekunden vor erneutem Versuch
        } else {
            std::cout << "Erfolgreich mit dem Server verbunden!" << std::endl;
            return true;
        }
    }
}

// Laserdaten lesen
std::string LaserScanClient::readLaserData() {
    char buffer[1024];
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

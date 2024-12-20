#ifndef LASER_SCAN_CLIENT_H
#define LASER_SCAN_CLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

//#pragma comment(lib, "Ws2_32.lib")

class LaserScanClient {
public:
    LaserScanClient(const std::string& ip, int port); // Konstruktor
    ~LaserScanClient(); // Destruktor

    bool connectToServer(); // Verbindung zum Server aufbauen
    std::string readLaserData(); // Laserdaten lesen

private:
    void initializeWinsock(); // Winsock initialisieren
    void createSocket(); // Socket erstellen
    void setupServerAddress(); // Server-Adresse einrichten
    void cleanup(); // Ressourcen aufräumen

    std::string ip_address; // Server-IP
    int port; // Portnummer
    SOCKET sock; // Socket
    sockaddr_in server_addr; // Server-Adresse
};

#endif // LASER_SCAN_CLIENT_H

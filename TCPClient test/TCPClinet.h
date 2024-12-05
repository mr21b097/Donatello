#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>

class TCPClient {
private:
    int sockfd;                 // Socket-Dateideskriptor
    bool connected;             // Verbindungsstatus
    static const int defaultPort = 5000; // Standardport (kann geändert werden)
    std::string ipAddress;      // Gespeicherte IP-Adresse

public:
    // Konstruktor: Verbindung zu einer IP-Adresse herstellen (Standardport wird verwendet)
    TCPClient(std::string ip);

    // Daten empfangen und ausgeben
    void receiveData();

    // Verbindung schließen
    ~TCPClient();
};

#endif // TCPCLIENT_H

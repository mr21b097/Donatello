#include <iostream>       // Für std::cout und std::cerr
#include <string>         // Für std::string
#include <cstring>        // Für std::memset
#include <winsock2.h>     // Für Winsock-Sockets
#include <ws2tcpip.h>     // Für inet_ntoa() und inet_pton()
#include <cstdlib>        // Für std::atoi() und std::exit()

#define MAXPENDING 5      // Maximale Anzahl an ausstehenden Verbindungsanforderungen

// Fehlerbehandlungsfunktion
void DieWithError(const std::string &errorMessage) {
    std::cerr << errorMessage << std::endl;
    WSACleanup();  // Winsock bereinigen
    std::exit(1);
}

// TCP-Client-Behandlungsfunktion
void HandleTCPClient(SOCKET clntSocket) {
    // Diese Funktion könnte implementiert werden, um mit dem Client zu kommunizieren
    // Hier kann beliebige Logik eingefügt werden, z.B. Empfang von Daten und Antworten
    std::string message = "Hello, Client!";
    send(clntSocket, message.c_str(), message.length(), 0);
    closesocket(clntSocket);
}

int main(int argc, char *argv[]) {
    WSADATA wsaData;               // Winsock-Daten
    SOCKET servSock;               // Socket-Deskriptor für den Server
    SOCKET clntSock;               // Socket-Deskriptor für den Client
    sockaddr_in echoServAddr;      // Lokale Adresse
    sockaddr_in echoClntAddr;      // Adresse des Clients
    unsigned short echoServPort;   // Port des Servers
    int clntLen;                   // Länge der Client-Adresse (hier int statt unsigned int)

    // Test auf die richtige Anzahl an Argumenten
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Server Port>" << std::endl;
        std::exit(1);
    }

    echoServPort = std::atoi(argv[1]);  // Erster Parameter: Port des Servers

    // Winsock initialisieren
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        DieWithError("WSAStartup() failed");
    }

    // Erstelle ein Socket für eingehende Verbindungen
    servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET) {
        DieWithError("socket() failed");
    }

    // Konstruiere die lokale Adressstruktur
    std::memset(&echoServAddr, 0, sizeof(echoServAddr));  // Setze Struktur auf Null
    echoServAddr.sin_family = AF_INET;                     // Internet-Adressfamilie
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);      // Jede eingehende Schnittstelle
    echoServAddr.sin_port = htons(echoServPort);           // Lokaler Port

    // Binde das Socket an die lokale Adresse
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) == SOCKET_ERROR) {
        DieWithError("bind() failed");
    }

    // Markiere das Socket als wartend auf eingehende Verbindungen
    if (listen(servSock, MAXPENDING) == SOCKET_ERROR) {
        DieWithError("listen() failed");
    }

    for (;;) {  // Endlosschleife für den Server
        // Setze die Größe des Eingabeparameters
        clntLen = sizeof(echoClntAddr);

        // Warte auf eine eingehende Client-Verbindung
        clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, (socklen_t*)&clntLen);  // Casting auf (socklen_t*)
        if (clntSock == INVALID_SOCKET) {
            DieWithError("accept() failed");
        }

        // clntSock ist jetzt mit einem Client verbunden
        std::cout << "Handling client " << inet_ntoa(echoClntAddr.sin_addr) << std::endl;

        HandleTCPClient(clntSock);  // Bearbeite die Verbindung mit dem Client
    }

    // Dieser Code wird nie erreicht, da die Schleife unendlich ist
    WSACleanup();  // Bereinigen der Winsock-Ressourcen
    return 0;
}

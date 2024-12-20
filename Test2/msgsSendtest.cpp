#include <iostream>     // für std::cerr und std::cout
#include <string>       // für std::string
#include <winsock2.h>   // für socket(), connect(), send(), recv(), WSAStartup(), WSACleanup()
#include <ws2tcpip.h>   // für inet_addr() und sockaddr_in
#include <cstdlib>      // für exit()
#include <sstream>      // für std::ostringstream

#define RCVBUFSIZE 20000   // Größe des Empfangspuffers

// Fehlerbehandlungsfunktion
void DieWithError(const std::string& errorMessage) {
    std::cerr << errorMessage << ". Fehlercode: " << WSAGetLastError() << std::endl;
    WSACleanup();  // Bereinigt Winsock
    exit(1);
}

int main(int argc, char* argv[]) {
    WSADATA wsaData;        // Wird für Winsock-Initialisierung benötigt
    int sock;               // Socket-Descriptor
    struct sockaddr_in echoServAddr;  // Adresse des Servers
    unsigned short echoServPort;      // Port des Servers
    std::string servIP;           // Server IP-Adresse (im Dotted-Quad-Format)
    char echoBuffer[RCVBUFSIZE];  // Puffer für die empfangene Nachricht
    int bytesRcvd, totalBytesRcvd;  // Anzahl der gelesenen Bytes in einem recv() und die Gesamtzahl der empfangenen Bytes

    // Überprüfen der Anzahl der Argumente
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <Server IP> <linear> <angular> [<Port>]" << std::endl;
        exit(1);
    }

    servIP = argv[1];             // Erstes Argument: Server IP-Adresse (Dotted Quad)
    float linear = std::stof(argv[2]);  // Zweites Argument: Linear-Wert
    float angular = std::stof(argv[3]); // Drittes Argument: Angular-Wert

    // Falls ein Port angegeben ist, verwenden wir diesen
    if (argc == 5)
        echoServPort = static_cast<unsigned short>(std::stoi(argv[4]));  // Benutze den angegebenen Port
    else
        echoServPort = 7;  // Standardport 7 für den Echo-Dienst

    // Nachricht generieren
    std::ostringstream messageStream;
    messageStream << "---START---{\"linear\": " << linear << ", \"angular\": " << angular << "}___END___";
    std::string messageToSend = messageStream.str();

    // Winsock initialisieren
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        DieWithError("WSAStartup() fehlgeschlagen");
    }

    // Erstelle einen zuverlässigen Stream-Socket (TCP)
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
        DieWithError("socket() fehlgeschlagen");

    // Konstruiere die Serveradresse-Struktur
    memset(&echoServAddr, 0, sizeof(echoServAddr));  // Setze die Struktur auf Null
    echoServAddr.sin_family      = AF_INET;           // Internet-Adressenfamilie
    echoServAddr.sin_addr.s_addr = inet_addr(servIP.c_str());  // Server IP-Adresse
    echoServAddr.sin_port        = htons(echoServPort);  // Serverport

    // Stelle die Verbindung zum Server her
    if (connect(sock, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) == SOCKET_ERROR)
        DieWithError("connect() fehlgeschlagen");

    // Sende die Nachricht an den Server
    if (send(sock, messageToSend.c_str(), messageToSend.length(), 0) != messageToSend.length())
        DieWithError("send() hat eine andere Anzahl von Bytes gesendet als erwartet");

    // Empfange die Antwort vom Server
    totalBytesRcvd = 0;
    std::cout << "Empfangen: ";  // Setup für das Ausgeben des empfangenen Strings
    while ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) > 0) {
        totalBytesRcvd += bytesRcvd;  // Zähle die Gesamtzahl der empfangenen Bytes
        echoBuffer[bytesRcvd] = '\0';  // Null-Terminierung des Strings
        std::cout << echoBuffer;       // Gib den empfangenen Echo-Puffer aus
    }

    if (bytesRcvd < 0)
        DieWithError("recv() fehlgeschlagen oder Verbindung unerwartet geschlossen");

    std::cout << std::endl;  // Ausgabe eines abschließenden Zeilenumbruchs

    // Schließe den Socket
    closesocket(sock);

    // Bereinige Winsock
    WSACleanup();

    return 0;
}

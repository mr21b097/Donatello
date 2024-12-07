#include <iostream>     // für std::cerr und std::cout
#include <string>       // für std::string
#include <winsock2.h>   // für socket(), connect(), send(), recv(), WSAStartup(), WSACleanup()
#include <ws2tcpip.h>   // für inet_addr() und sockaddr_in
#include <cstdlib>      // für exit()

#define RCVBUFSIZE 1024   // Größe des Empfangspuffers

// Fehlerbehandlungsfunktion
void DieWithError(const std::string& errorMessage) {
    std::cerr << errorMessage << ". Fehlercode: " << WSAGetLastError() << std::endl;
    WSACleanup();  // Bereinigt Winsock
    exit(1);
}

int main(int argc, char* argv[]) {
    WSADATA wsaData;        // Wird für Winsock-Initialisierung benötigt
    int sock;               // Socket-Descriptor
    struct sockaddr_in echoServAddr;  // Adresse des Echo-Servers
    unsigned short echoServPort;      // Port des Echo-Servers
    std::string servIP;           // Server IP-Adresse (im Dotted-Quad-Format)
    std::string echoString;       // String, der an den Echo-Server gesendet wird
    char echoBuffer[RCVBUFSIZE];  // Puffer für den Echo-String
    unsigned int echoStringLen;   // Länge des zu sendenden Strings
    int bytesRcvd, totalBytesRcvd;  // Anzahl der gelesenen Bytes in einem recv() und die Gesamtzahl der empfangenen Bytes

    // Überprüfen der Anzahl der Argumente
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <Server IP> <Echo Word> [<Echo Port>]" << std::endl;
        exit(1);
    }

    servIP = argv[1];             // Erstes Argument: Server IP-Adresse (Dotted Quad)
    echoString = argv[2];         // Zweites Argument: String, der gesendet werden soll

    // Falls ein drittes Argument (Port) angegeben ist, verwenden wir diesen
    if (argc == 4)
        echoServPort = static_cast<unsigned short>(std::stoi(argv[3]));  // Benutze den angegebenen Port
    else
        echoServPort = 7;  // Standardport 7 für den Echo-Dienst

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

    // Stelle die Verbindung zum Echo-Server her
    if (connect(sock, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) == SOCKET_ERROR)
        DieWithError("connect() fehlgeschlagen");

    echoStringLen = echoString.length();  // Bestimme die Länge des zu sendenden Strings

    // Sende den String an den Server
    if (send(sock, echoString.c_str(), echoStringLen, 0) != echoStringLen)
        DieWithError("send() hat eine andere Anzahl von Bytes gesendet als erwartet");

    // Empfange denselben String zurück vom Server
    totalBytesRcvd = 0;
    std::cout << "Empfangen: ";  // Setup für das Ausgeben des empfangenen Strings
    while (totalBytesRcvd < echoStringLen) {
        // Empfange bis zu der Puffergöße (minus 1 für das Null-Terminator)
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() fehlgeschlagen oder Verbindung unerwartet geschlossen");
        totalBytesRcvd += bytesRcvd;  // Zähle die Gesamtzahl der empfangenen Bytes
        echoBuffer[bytesRcvd] = '\0';  // Null-Terminierung des Strings
        std::cout << echoBuffer;       // Gib den empfangenen Echo-Puffer aus
    }

    std::cout << std::endl;  // Ausgabe eines abschließenden Zeilenumbruchs

    // Schließe den Socket
    closesocket(sock);

    // Bereinige Winsock
    WSACleanup();

    return 0;
}

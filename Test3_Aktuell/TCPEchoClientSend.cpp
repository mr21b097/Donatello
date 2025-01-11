#include <iostream>     // Für std::cerr und std::cout
#include <string>       // Für std::string
#include <winsock2.h>   // Für socket(), connect(), send(), recv(), WSAStartup(), WSACleanup()
#include <ws2tcpip.h>   // Für inet_addr() und sockaddr_in
#include <cstdlib>      // Für exit()
#include <sstream>      // Für std::ostringstream

// Fehlerbehandlungsfunktion
void DieWithError(const std::string& errorMessage) {
    std::cerr << errorMessage << ". Fehlercode: " << WSAGetLastError() << std::endl;
    WSACleanup();  // Bereinigt Winsock
    exit(1);
}

int main(int argc, char* argv[]) {
    WSADATA wsaData;               // Wird für Winsock-Initialisierung benötigt
    SOCKET sock;                   // Socket-Descriptor
    struct sockaddr_in echoServAddr; // Adresse des Echo-Servers
    unsigned short echoServPort;   // Port des Echo-Servers
    std::string servIP;            // Server IP-Adresse (im Dotted-Quad-Format)
    std::string messageToSend;     // String, der an den Echo-Server gesendet wird
    unsigned int messageLength;    // Länge des zu sendenden Strings

    // Überprüfen der Anzahl der Argumente
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <Server IP> <Linear> <Angular> <Port>" << std::endl;
        exit(1);
    }

    servIP = argv[1];                     // Erstes Argument: Server IP-Adresse
    double linear = std::stod(argv[2]);   // Zweites Argument: Linearwert
    double angular = std::stod(argv[3]);  // Drittes Argument: Angularwert
    echoServPort = static_cast<unsigned short>(std::stoi(argv[4]));  // Viertes Argument: Port

    // Nachricht im gewünschten Format erstellen
    std::ostringstream oss;
    oss << "---START---{\"linear\": " << linear << ", \"angular\": " << angular << "}___END___";
    messageToSend = oss.str();

    // Winsock initialisieren
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        DieWithError("WSAStartup() fehlgeschlagen");
    }

    // Erstelle einen zuverlässigen Stream-Socket (TCP)
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        DieWithError("socket() fehlgeschlagen");
    }

    // Konstruiere die Serveradresse-Struktur
    memset(&echoServAddr, 0, sizeof(echoServAddr));  // Setze die Struktur auf Null
    echoServAddr.sin_family      = AF_INET;         // Internet-Adressenfamilie
    echoServAddr.sin_addr.s_addr = inet_addr(servIP.c_str());  // Server IP-Adresse
    echoServAddr.sin_port        = htons(echoServPort);        // Serverport

    // Stelle die Verbindung zum Echo-Server her
    if (connect(sock, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) == SOCKET_ERROR) {
        DieWithError("connect() fehlgeschlagen");
    }

    messageLength = messageToSend.length();  // Bestimme die Länge des zu sendenden Strings

    // Sende den String an den Server
    if (send(sock, messageToSend.c_str(), static_cast<int>(messageLength), 0) != static_cast<int>(messageLength)) {
        DieWithError("send() hat eine andere Anzahl von Bytes gesendet als erwartet");
    }

    std::cout << "Nachricht gesendet: " << messageToSend << std::endl;

    // Schließe den Socket
    closesocket(sock);

    // Bereinige Winsock
    WSACleanup();

    return 0;
}

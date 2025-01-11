#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

// Funktion zum Starten einer Exe-Datei ohne Ausgabeumleitung (z.B. für Server)
void runExecutableNoOutput(const std::string& exePath, const std::string& arguments) {
    std::string commandLine = exePath + " " + arguments;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Prozess starten
    if (!CreateProcess(
            NULL,
            &commandLine[0],
            NULL,
            NULL,
            FALSE, // Keine Vererbung der Handles
            0,
            NULL,
            NULL,
            &si,
            &pi)) {
        std::cerr << "Fehler beim Starten von " << exePath << ": " << GetLastError() << std::endl;
        return;
    }

    // Auf Beendigung des Prozesses warten
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Handles schließen
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// Funktion zum Starten einer Exe-Datei mit Ausgabeumleitung (z.B. für Clients)
std::string runExecutableWithOutput(const std::string& exePath, const std::string& arguments) {
    std::string commandLine = exePath + " " + arguments;

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    // Pipe erstellen
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        std::cerr << "Fehler beim Erstellen der Pipe: " << GetLastError() << std::endl;
        return "";
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi = { 0 };

    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe; // Fehlerausgabe auch umleiten
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Prozess starten
    if (!CreateProcess(
            NULL,
            &commandLine[0],
            NULL,
            NULL,
            TRUE, // Vererbung der Handles erlauben
            0,
            NULL,
            NULL,
            &si,
            &pi)) {
        std::cerr << "Fehler beim Starten von " << exePath << ": " << GetLastError() << std::endl;
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return "";
    }

    CloseHandle(hWritePipe); // Schreib-Ende der Pipe schließen

    // Ausgabe aus der Pipe lesen
    std::string output;
    char buffer[4096];
    DWORD bytesRead;

    while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        output += buffer;
    }

    // Handles schließen
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return output;
}

int main() {
    // Exe-Pfade anpassen
    std::string serverExePath = ".\\Test3_Aktuell\\server.exe";
    std::string clientExePath = ".\\Test3_Aktuell\\client.exe";
    std::string laserANAExePath = ".\\Test3_Aktuell\\laser_analysis.exe";
    std::string odomANAExePath = ".\\Test3_Aktuell\\odom_analysis.exe";

    // Argumente für die Programme
    std::string serverArgs1 = "9997";
    std::string serverArgs2 = "9998";
    std::string clientArgs1 = "192.168.100.54 TestMessage 9997";
    std::string clientArgs2 = "192.168.100.54 TestMessage 9998";

    // Threads erstellen
    std::thread serverThread1(runExecutableNoOutput, serverExePath, serverArgs1);
    std::thread serverThread2(runExecutableNoOutput, serverExePath, serverArgs2);
    

    // Warten, bis die Threads beendet sind
    //serverThread1.join();
    //serverThread2.join();

    while(true)
    {
    std::string clientOutput1, clientOutput2;
    std::thread clientThread1([&]() { clientOutput1 = runExecutableWithOutput(clientExePath, clientArgs1); });
    std::thread clientThread2([&]() { clientOutput2 = runExecutableWithOutput(clientExePath, clientArgs2); });

    std::string Laser_ANA_Args = clientOutput1;
    std::string Odom_ANA_Args = clientOutput2;

    std::string laserANAOutput, odomANAOutput;
    std::thread LaserANAThread([&]() { laserANAOutput = runExecutableWithOutput(laserANAExePath, clientArgs1); });
    std::thread OdomANAThread([&]() { odomANAOutput = runExecutableWithOutput(odomANAExePath, clientArgs2); });

    // Warten, bis die Threads beendet sind
    clientThread1.join();
    clientThread2.join();
    OdomANAThread.join();
    LaserANAThread.join();

    // Ausgabe der Clients anzeigen
    std::cout << "Client 1 Ausgabe:\n" << laserANAOutput << "\n" << std::endl;
    std::cout << "Client 2 Ausgabe:\n" << odomANAOutput << "\n" << std::endl;
    }
    return 0;
}

#include <iostream>
#include <string>
#include <thread>
#include <regex>
#include <vector>
#include <windows.h>


//TEST ODOM ANALYSE
/*
struct Pose {
    struct Position {
        double x, y, z;
    } position;

    struct Orientation {
        double x, y, z, w;
    } orientation;
};

struct Twist {
    struct Linear {
        double x, y, z;
    } linear;

    struct Angular {
        double x, y, z;
    } angular;
};

struct RobotState {
    Pose pose;
    Twist twist;
};

RobotState parseRobotState(const std::string& data) {
    RobotState state = {};

    // Regex für Position, Orientierung, Linear und Angular
    std::regex positionRegex("\"position\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+)\\s*\\}");
    std::regex orientationRegex("\"orientation\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+),\\s*\"w\":\\s*([-\\d\\.eE]+)\\s*\\}");
    std::regex linearRegex("\"linear\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+)\\s*\\}");
    std::regex angularRegex("\"angular\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+)\\s*\\}");

    std::smatch match;

    // Position parsen
    if (std::regex_search(data, match, positionRegex)) {
        state.pose.position.x = std::stod(match[1]);
        state.pose.position.y = std::stod(match[2]);
        state.pose.position.z = std::stod(match[3]);
    } else {
        std::cerr << "Position nicht gefunden!" << std::endl;
    }

    // Orientierung parsen
    if (std::regex_search(data, match, orientationRegex)) {
        state.pose.orientation.x = std::stod(match[1]);
        state.pose.orientation.y = std::stod(match[2]);
        state.pose.orientation.z = std::stod(match[3]);
        state.pose.orientation.w = std::stod(match[4]);
    } else {
        std::cerr << "Orientierung nicht gefunden!" << std::endl;
    }

    // Lineare Geschwindigkeit parsen
    if (std::regex_search(data, match, linearRegex)) {
        state.twist.linear.x = std::stod(match[1]);
        state.twist.linear.y = std::stod(match[2]);
        state.twist.linear.z = std::stod(match[3]);
    } else {
        std::cerr << "Lineare Geschwindigkeit nicht gefunden!" << std::endl;
    }

    // Angulare Geschwindigkeit parsen
    if (std::regex_search(data, match, angularRegex)) {
        state.twist.angular.x = std::stod(match[1]);
        state.twist.angular.y = std::stod(match[2]);
        state.twist.angular.z = std::stod(match[3]);
    } else {
        std::cerr << "Angulare Geschwindigkeit nicht gefunden!" << std::endl;
    }

    return state;
}
*/
//ENDE TEST ODOM ADALYSE


// Funktion zum Starten einer Exe-Datei ohne Ausgabeumleitung (z.B. für Server)
//QUELLE: https://learn.microsoft.com/de-de/windows/win32/procthread/processes-and-threads
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
//QUELLE: https://learn.microsoft.com/de-de/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
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
    std::string serverExePath = "C:\\Users\\Christoph Roth\\Desktop\\APR\\Donatello\\Test3_Aktuell\\server.exe";
    std::string clientExePath = "C:\\Users\\Christoph Roth\\Desktop\\APR\\Donatello\\Test3_Aktuell\\client.exe";
    std::string laserANAExePath = "C:\\Users\\Christoph Roth\\Desktop\\APR\\Donatello\\Test3_Aktuell\\laser_analysis.exe";
    std::string odomANAExePath = "C:\\Users\\Christoph Roth\\Desktop\\APR\\Donatello\\Test3_Aktuell\\odom_analysis.exe";

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

    // Warten, bis die Threads beendet sind
    clientThread1.join();
    clientThread2.join();

    // Ausgabe der Clients anzeigen
   // std::cout << "Client 1 Ausgabe:\n" << clientOutput1 << "\n" << std::endl;
   // std::cout << "Client 2 Ausgabe:\n" << clientOutput2 << "\n" << std::endl;

 
//TEST Laser Analyse 
/*
    std::string input = clientOutput1;

    // Marker definitions
    std::string startMarker = "---START---";
    std::string endMarker = "___END___";

    // Check if both markers exist
    size_t startPos = input.find(startMarker);
    size_t endPos = input.find(endMarker);

    if (startPos != std::string::npos && endPos != std::string::npos) {
        // Extract the data between markers
        startPos += startMarker.length();
        std::string jsonData = input.substr(startPos, endPos - startPos);

        // Find the ranges array in the JSON data
        std::regex rangesRegex("\"ranges\":\\s*\\[(.*?)\\]");
        std::smatch matches;
        if (std::regex_search(jsonData, matches, rangesRegex)) {
            std::string rangesString = matches[1].str();

            // Parse the ranges into a vector
            std::vector<float> ranges;
            std::regex numberRegex("-?\\d+(\\.\\d+)?");
            auto numbersBegin = std::sregex_iterator(rangesString.begin(), rangesString.end(), numberRegex);
            auto numbersEnd = std::sregex_iterator();

            for (std::sregex_iterator i = numbersBegin; i != numbersEnd; ++i) {
                ranges.push_back(std::stof((*i).str()));
            }

            // Check if the ranges array has exactly 360 values
            if (ranges.size() == 360) {
                std::cout << "Extracted ranges:" << std::endl;
                for (float range : ranges) {
                    std::cout << range << " ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "Error: Ranges array does not contain exactly 360 values. Found: " << ranges.size() << std::endl;
            }
        } else {
            std::cout << "Ranges array not found in the data." << std::endl;
        }
    } else {
        std::cout << "Markers not found in the input string." << std::endl;
    }


//ende Laser Analyse 
*/

//Test odom analyse
/*

    // Beispiel-Daten mit Start und End
    std::string data = clientOutput2;

    // Überprüfen, ob Start und End vorhanden sind
   // size_t startPos = data.find("---START---");
   // size_t endPos = data.find("___END___");

    if (startPos != std::string::npos && endPos != std::string::npos) {
        // Extrahiere den relevanten JSON-Teil
        std::string jsonData = data.substr(startPos + 11, endPos - (startPos + 11));

        // Roboterstatus-Daten extrahieren
        RobotState state = parseRobotState(jsonData);

        // Ergebnisse ausgeben
        std::cout << "Position: (" 
                  << state.pose.position.x << ", " 
                  << state.pose.position.y << ", " 
                  << state.pose.position.z << ")" << std::endl;

        std::cout << "Orientation: (" 
                  << state.pose.orientation.x << ", " 
                  << state.pose.orientation.y << ", " 
                  << state.pose.orientation.z << ", " 
                  << state.pose.orientation.w << ")" << std::endl;

        std::cout << "Linear Velocity: (" 
                  << state.twist.linear.x << ", " 
                  << state.twist.linear.y << ", " 
                  << state.twist.linear.z << ")" << std::endl;

        std::cout << "Angular Velocity: (" 
                  << state.twist.angular.x << ", " 
                  << state.twist.angular.y << ", " 
                  << state.twist.angular.z << ")" << std::endl;
    } else {
        std::cerr << "Fehler: Start oder End Marker fehlt!" << std::endl;
    }

//Ende ztest odom analyse
*/

    }
    return 0;
}
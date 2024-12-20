#include <iostream>
#include <thread>
#include <atomic>
#include "LaserScanClient.h"
#include "OdometryClient.h"
#include "VelocityClient.h"
#include "SharedMemory.h"

// Globale Variable, um die Threads sauber zu beenden
std::atomic<bool> running(true);

// Funktion, um Laser-Scan-Daten zu lesen
void readLaserScan(LaserScanClient& laserClient) {
    while (running) {
        laserClient.readLaserData();  // Implementiere die Methode in LaserScanClient
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Funktion, um Odometrie-Daten zu lesen
void readOdometry(OdometryClient& odometryClient) {
    while (running) {
        odometryClient.readOdometryData();  // Implementiere die Methode in OdometryClient
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Funktion, um Geschwindigkeitskommandos zu senden
void sendVelocity(VelocityClient& velocityClient) {
    while (running) {
        float linear = 0.5;  // Beispielwerte
        float angular = 0.1;
        velocityClient.sendVelocityCommand(linear, angular);  // Implementiere die Methode in VelocityClient
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    try {
        // Shared Memory initialisieren
        SharedMemory sharedMemory;

        // Verbindungen zu den Servern herstellen
        LaserScanClient laserClient("192.168.100.54", 9997);
        OdometryClient odometryClient("192.168.100.54", 9998);
        VelocityClient velocityClient("192.168.100.54", 9999);

        laserClient.connectToServer();
        odometryClient.connectToServer();
        velocityClient.connectToServer();

        // Threads für die jeweiligen Funktionen starten
        std::thread laserThread(readLaserScan, std::ref(laserClient));
        std::thread odometryThread(readOdometry, std::ref(odometryClient));
        std::thread velocityThread(sendVelocity, std::ref(velocityClient));

        // Warten, bis der Benutzer die Eingabetaste drückt, um die Threads zu beenden
        std::cout << "Press Enter to stop...\n";
        std::cin.get();

        // Stop-Signal setzen
        running = false;

        // Auf die Beendigung der Threads warten
        if (laserThread.joinable()) laserThread.join();
        if (odometryThread.joinable()) odometryThread.join();
        if (velocityThread.joinable()) velocityThread.join();

        std::cout << "Program terminated successfully.\n";

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

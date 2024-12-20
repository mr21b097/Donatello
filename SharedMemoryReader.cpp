#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <cstring> // Für String-Operationen wie std::strcpy

struct SharedData {
    char laserData[1024]; // Platz für Laser-Daten
    char odomData[256];   // Platz für Odometrie-Daten
};

// Globale Variablen
HANDLE sem_read = NULL;          // Handle für die Lesesemaphore
HANDLE sem_write = NULL;         // Handle für die Schreibsemaphore
HANDLE sharedMemoryHandle = NULL;// Handle für den Shared Memory
SharedData* sharedMemory = NULL; // Zeiger auf den Shared Memory

void attachSharedMemory() {
    // Existierende Semaphore für Lesen öffnen
    sem_read = OpenSemaphoreA(SYNCHRONIZE, FALSE, "SemRead");
    if (!sem_read) {
        throw std::runtime_error("Failed to open read semaphore");
    }

    // Existierende Semaphore für Schreiben öffnen
    sem_write = OpenSemaphoreA(SYNCHRONIZE, FALSE, "SemWrite");
    if (!sem_write) {
        throw std::runtime_error("Failed to open write semaphore");
    }

    // Existierenden Shared Memory öffnen
    sharedMemoryHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "SharedMemory");
    if (!sharedMemoryHandle) {
        throw std::runtime_error("Failed to open shared memory");
    }

    // Shared Memory in den Adressraum des Prozesses einblenden
    sharedMemory = static_cast<SharedData*>(MapViewOfFile(
        sharedMemoryHandle,   // Handle des Shared Memory
        FILE_MAP_ALL_ACCESS,  // Vollzugriff auf den Speicherbereich
        0,                    // Offset High-Order-Bits (nicht benötigt)
        0,                    // Offset Low-Order-Bits (nicht benötigt)
        sizeof(SharedData)    // Größe des zu mappenden Bereichs
    ));
    if (!sharedMemory) {
        throw std::runtime_error("Failed to map shared memory");
    }

    std::cout << "Attached to shared memory!" << std::endl;
}

void detachSharedMemory() {
    if (sharedMemory) {
        // Entfernt die Zuordnung des Shared Memory aus dem Prozessspeicher
        UnmapViewOfFile(sharedMemory);
        sharedMemory = NULL;
    }

    if (sharedMemoryHandle) {
        // Schließt das Handle für den Shared Memory
        CloseHandle(sharedMemoryHandle);
        sharedMemoryHandle = NULL;
    }

    if (sem_read) {
        // Schließt das Handle für die Lesesemaphore
        CloseHandle(sem_read);
        sem_read = NULL;
    }

    if (sem_write) {
        // Schließt das Handle für die Schreibsemaphore
        CloseHandle(sem_write);
        sem_write = NULL;
    }

    std::cout << "Detached from shared memory!" << std::endl;
}

int main() {
    try {
        // Verbindung zu existierendem Shared Memory und Semaphoren herstellen
        attachSharedMemory();

        // Warten, bis Leserechte verfügbar sind
        WaitForSingleObject(sem_read, INFINITE);

        // Daten aus dem Shared Memory lesen
        std::cout << "Data read from shared memory: " 
                  << sharedMemory->laserData << ", " 
                  << sharedMemory->odomData << std::endl;

        // Signalisieren, dass der Schreibzugriff möglich ist
        ReleaseSemaphore(sem_write, 1, NULL);

        // Ressourcen freigeben
        detachSharedMemory();
    } catch (const std::exception& e) {
        // Fehlerbehandlung: Fehler ausgeben und Ressourcen freigeben
        std::cerr << "Error: " << e.what() << std::endl;
        detachSharedMemory();
        return 1; // Fehlercode zurückgeben
    }

    return 0; // Erfolgreicher Programmabschluss
}

#include <windows.h>     // Für Windows-API-Funktionen wie CreateSemaphoreA und CreateFileMappingA
#include <iostream>      // Für Ein- und Ausgabe
#include <stdexcept>     // Für die Verwendung von std::runtime_error
#include <cstring>       // Für String-Operationen wie std::strcpy

// Struktur für die Daten, die im Shared Memory gespeichert werden sollen
struct SharedData {
    char laserData[1024]; // Platz für Laser-Daten
    char odomData[256];   // Platz für Odometrie-Daten
};

// Globale Handles für Semaphoren und Shared Memory
HANDLE sem_read = NULL;          // Semaphore zum Signalisieren, dass Daten gelesen werden können
HANDLE sem_write = NULL;         // Semaphore zum Signalisieren, dass Daten geschrieben werden können
HANDLE sharedMemoryHandle = NULL;// Handle für den Shared-Memory-Bereich
SharedData* sharedMemory = NULL; // Zeiger auf den Shared-Memory-Bereich

// Funktion zur Initialisierung von Shared Memory und Semaphoren
void initSharedMemory() {
    // Semaphore für Lesen erstellen
    sem_read = CreateSemaphoreA(
        NULL,    // Keine spezielle Sicherheitsattribute
        1,       // Initialer Zählerwert (1 = verfügbar)
        1,       // Maximaler Zählerwert (1 = binäre Semaphore)
        "SemRead"// Name der Semaphore (wird systemweit geteilt)
    );
    if (!sem_read) {
        throw std::runtime_error("Failed to create read semaphore");
    }

    // Semaphore für Schreiben erstellen
    sem_write = CreateSemaphoreA(
        NULL,     // Keine spezielle Sicherheitsattribute
        1,        // Initialer Zählerwert (1 = verfügbar)
        1,        // Maximaler Zählerwert (1 = binäre Semaphore)
        "SemWrite"// Name der Semaphore (wird systemweit geteilt)
    );
    if (!sem_write) {
        throw std::runtime_error("Failed to create write semaphore");
    }

    // Shared Memory erstellen
    sharedMemoryHandle = CreateFileMappingA(
        INVALID_HANDLE_VALUE, // Kein physischer Dateibackend, nur Speicher
        NULL,                 // Standard-Sicherheitsattribute
        PAGE_READWRITE,       // Lese-/Schreibzugriff erlauben
        0,                    // High-Order-Bits für die Größe (nicht benötigt)
        sizeof(SharedData),   // Größe des Shared Memory
        "SharedMemory"        // Name des Shared Memory (wird systemweit geteilt)
    );
    if (!sharedMemoryHandle) {
        throw std::runtime_error("Failed to create shared memory");
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

    // Speicher initialisieren (alle Bytes auf 0 setzen)
    memset(sharedMemory->laserData, 0, sizeof(sharedMemory->laserData));
    memset(sharedMemory->odomData, 0, sizeof(sharedMemory->odomData));

    std::cout << "Shared memory initialized!" << std::endl;
}

// Funktion zum Aufräumen der Ressourcen
void cleanupSharedMemory() {
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

    std::cout << "Shared memory cleaned up!" << std::endl;
}

// Hauptfunktion
int main() {
    try {
        // Initialisiert Shared Memory und Semaphoren
        initSharedMemory();

        while (true) {  // Endlosschleife zum kontinuierlichen Schreiben
            // Schreibzugriff auf Shared Memory sichern
            WaitForSingleObject(sem_write, INFINITE); // Warten, bis Schreibzugriff erlaubt ist

            // Beispiel-Daten in den Shared Memory schreiben
            std::strcpy(sharedMemory->laserData, "Example laser data"); // Daten kopieren
            std::strcpy(sharedMemory->odomData, "Example odom data");   // Daten kopieren

            // Signalisieren, dass Lesen möglich ist
            ReleaseSemaphore(sem_read, 1, NULL);

            // Zeigt die geschriebenen Daten an
            std::cout << "Data written to shared memory: "
                      << sharedMemory->laserData << ", " << sharedMemory->odomData << std::endl;

            // Eine kurze Pause (optional), um den Prozess nicht zu überlasten
            Sleep(1000); // 1 Sekunde warten
        }

        // Ressourcen aufräumen (dieser Code wird in einer Endlosschleife nicht erreicht)
        cleanupSharedMemory();
    } catch (const std::exception& e) {
        // Fehlerbehandlung: Fehler ausgeben und Ressourcen freigeben
        std::cerr << "Error: " << e.what() << std::endl;
        cleanupSharedMemory();
        return 1; // Fehlercode zurückgeben
    }

    return 0; // Erfolgreicher Programmabschluss
}

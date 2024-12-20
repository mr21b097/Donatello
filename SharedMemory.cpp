#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <cstring>

struct SharedData {
    char laserData[1024];
    char odomData[256];
};

HANDLE sem_read = NULL;          
HANDLE sem_write = NULL;         
HANDLE sharedMemoryHandle = NULL;
SharedData* sharedMemory = NULL;

void initSharedMemory() {
    sem_read = CreateSemaphoreA(NULL, 0, 1, "SemRead");  // Initialwert: 0 (Leseprozess wartet)
    if (!sem_read) {
        throw std::runtime_error("Failed to create read semaphore");
    }

    sem_write = CreateSemaphoreA(NULL, 1, 1, "SemWrite");  // Initialwert: 1 (Schreibprozess kann starten)
    if (!sem_write) {
        throw std::runtime_error("Failed to create write semaphore");
    }

    sharedMemoryHandle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedData), "SharedMemory");
    if (!sharedMemoryHandle) {
        throw std::runtime_error("Failed to create shared memory");
    }

    sharedMemory = static_cast<SharedData*>(MapViewOfFile(sharedMemoryHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData)));
    if (!sharedMemory) {
        throw std::runtime_error("Failed to map shared memory");
    }

    memset(sharedMemory->laserData, 0, sizeof(sharedMemory->laserData));
    memset(sharedMemory->odomData, 0, sizeof(sharedMemory->odomData));
    std::cout << "Shared memory initialized!" << std::endl;
}

void cleanupSharedMemory() {
    if (sharedMemory) {
        UnmapViewOfFile(sharedMemory);
        sharedMemory = NULL;
    }

    if (sharedMemoryHandle) {
        CloseHandle(sharedMemoryHandle);
        sharedMemoryHandle = NULL;
    }

    if (sem_read) {
        CloseHandle(sem_read);
        sem_read = NULL;
    }

    if (sem_write) {
        CloseHandle(sem_write);
        sem_write = NULL;
    }

    std::cout << "Shared memory cleaned up!" << std::endl;
}

int main() {
    try {
        initSharedMemory();

        ReleaseSemaphore(sem_write, 1, NULL);
        ReleaseSemaphore(sem_read, 1, NULL);

        while (true) {
            std::cout << "Writer waiting for semaphore..." << std::endl;
            // Warten, dass der Leseprozess die Daten liest
            WaitForSingleObject(sem_write, INFINITE);  // Warten, bis der Writer schreiben darf

            // Beispiel-Daten in den Shared Memory schreiben
            std::strcpy(sharedMemory->laserData, "Example laser data");
            std::strcpy(sharedMemory->odomData, "Example odom data");

            // Gebe das Lese-Semaphor frei, damit der Leseprozess auf die neuen Daten zugreifen kann
            std::cout << "Writer releasing semaphore..." << std::endl;
            ReleaseSemaphore(sem_read, 1, NULL);
            
           // ReleaseSemaphore(sem_write, 1, NULL); //Für Endlosschleife jedoch ohne Synchronisation 

            std::cout << "Data written to shared memory: "
                      << sharedMemory->laserData << ", " << sharedMemory->odomData << std::endl;

            Sleep(1000);  // 1 Sekunde warten
        }

        cleanupSharedMemory();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        cleanupSharedMemory();
        return 1;
    }

    return 0;
}

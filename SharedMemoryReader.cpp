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

void attachSharedMemory() {
    sem_read = OpenSemaphoreA(SYNCHRONIZE, FALSE, "SemRead");
    if (!sem_read) {
        throw std::runtime_error("Failed to open read semaphore");
    }

    sem_write = OpenSemaphoreA(SYNCHRONIZE, FALSE, "SemWrite");
    if (!sem_write) {
        throw std::runtime_error("Failed to open write semaphore");
    }

    sharedMemoryHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "SharedMemory");
    if (!sharedMemoryHandle) {
        throw std::runtime_error("Failed to open shared memory");
    }

    sharedMemory = static_cast<SharedData*>(MapViewOfFile(sharedMemoryHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData)));
    if (!sharedMemory) {
        throw std::runtime_error("Failed to map shared memory");
    }

    std::cout << "Attached to shared memory!" << std::endl;
}

void detachSharedMemory() {
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

    std::cout << "Detached from shared memory!" << std::endl;
}

int main() {
    try {
        attachSharedMemory();

        while (true) {
            // Warten, bis Daten geschrieben wurden
            WaitForSingleObject(sem_read, INFINITE);

            // Daten aus dem Shared Memory lesen
            std::cout << "Data read from shared memory: "
                      << sharedMemory->laserData << ", "
                      << sharedMemory->odomData << std::endl;

            // Gebe das Schreib-Semaphor frei
            ReleaseSemaphore(sem_write, 1, NULL);

            // Hier kannst du eine Pause einfügen, wenn du möchtest
            Sleep(1000); // 1 Sekunde warten
        }

        detachSharedMemory();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        detachSharedMemory();
        return 1;
    }

    return 0;
}

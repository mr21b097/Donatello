#include <iostream>
#include <windows.h>

struct SharedData {
    float laser_data[1024];
    float odometry_data[1024];
    HANDLE sem;
};

class SharedMemory {
public:
    SharedMemory() {
        // Shared Memory erstellen
        shm_handle = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(SharedData), nullptr);
        if (shm_handle == nullptr) {
            std::cerr << "Unable to create file mapping!" << std::endl;
            return;
        }

        shared_memory = (SharedData*)MapViewOfFile(shm_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
        if (shared_memory == nullptr) {
            std::cerr << "Unable to map view of file!" << std::endl;
            return;
        }

        // Semaphore erstellen
        sem = CreateSemaphore(nullptr, 1, 1, nullptr);  // Initialisieren auf 1
        if (sem == nullptr) {
            std::cerr << "Unable to create semaphore!" << std::endl;
        }
    }

    ~SharedMemory() {
        if (sem) CloseHandle(sem);
        if (shared_memory) UnmapViewOfFile(shared_memory);
        if (shm_handle) CloseHandle(shm_handle);
    }

    SharedData* getSharedData() {
        return shared_memory;
    }

private:
    HANDLE shm_handle;
    SharedData* shared_memory;
    HANDLE sem;
};

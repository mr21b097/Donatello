#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <windows.h>

struct SharedData {
    float laser_data[1024];
    float odometry_data[1024];
    HANDLE sem;
};

class SharedMemory {
public:
    SharedMemory();
    ~SharedMemory();
    SharedData* getSharedData();

private:
    HANDLE shm_handle;
    SharedData* shared_memory;
    HANDLE sem;
};

#endif // SHARED_MEMORY_H

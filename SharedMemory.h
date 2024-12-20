#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <windows.h>
#include <string>

struct SharedData {
    char laserData[1024];
    char odomData[256];
};

extern HANDLE sem_read;
extern HANDLE sem_write;
extern HANDLE sharedMemoryHandle;
extern SharedData* sharedMemory;

void initSharedMemory();
void cleanupSharedMemory();

#endif

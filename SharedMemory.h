#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

struct SharedData {
    char laserData[1024];
    char odomData[256];
};

extern sem_t* sem_read;
extern sem_t* sem_write;
extern SharedData* sharedMemory;

void initSharedMemory();
void cleanupSharedMemory();

#endif

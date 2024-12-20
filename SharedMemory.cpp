#include "SharedMemory.h"

sem_t* sem_read = nullptr;
sem_t* sem_write = nullptr;
SharedData* sharedMemory = nullptr;

void initSharedMemory() {
    int fd = shm_open("/shared_mem", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));
    sharedMemory = static_cast<SharedData*>(mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    sem_read = sem_open("/sem_read", O_CREAT, 0666, 0);
    sem_write = sem_open("/sem_write", O_CREAT, 0666, 1);
}

void cleanupSharedMemory() {
    munmap(sharedMemory, sizeof(SharedData));
    shm_unlink("/shared_mem");
    sem_close(sem_read);
    sem_close(sem_write);
    sem_unlink("/sem_read");
    sem_unlink("/sem_write");
}

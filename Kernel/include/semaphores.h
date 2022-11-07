#ifndef TP2_SO_SEMAPHORES_H
#define TP2_SO_SEMAPHORES_H

#include <stdint.h>

#define MAX_BLOCKED_PROCESSES 15

typedef struct semaphore {
    uint32_t id;
    uint64_t value;
    int blockedProcesses[MAX_BLOCKED_PROCESSES];
    uint16_t blockedProcessesAmount;
    uint16_t listeningProcesses;
    struct semaphore *next;
    int lock;
} semaphore;

typedef struct semaphoreData {
    uint32_t id;
    uint64_t value;
    int blockedProcesses[MAX_BLOCKED_PROCESSES];
    uint16_t blockedProcessesAmount;
    uint16_t listeningProcesses;
} semaphoreData;

int semOpen(uint32_t id, uint64_t initialValue);
int semWait(uint32_t id);
int semPost(uint32_t id);
int semClose(uint32_t id);
int semaphoreInfo(semaphoreData * s);
#endif //TP2_SO_SEMAPHORES_H

#include "../include/semaphores.h"
#include "../include/lib.h"

#define NULL ((void *)0)

static semaphore *getSemaphore(uint32_t id);
static semaphore *createSemaphore(uint32_t id, uint64_t initialValue);
static void addSemaphoreToList(semaphore *newSem);
static void unblockSemProcess(semaphore *sem);
static void removeSemaphore(semaphore *sem);
static void blockedProcessesDump(int *blockedProcesses,
                                 uint16_t blockedProcessesAmount);

int semOpen(uint32_t id, uint64_t initialValue) {
    semaphore *sem= getSemaphore(id);

    if (sem== NULL) {
        sem = createSemaphore(id, initialValue);
        if (sem == NULL) {
            return -1;
        }
    }

    if (sem->listeningProcesses >= MAX_BLOCKED_PROCESSES) {
        return -1;
    }

    sem->listeningProcesses++;
    return id;
}

int semWait(uint32_t id) {
    semaphore *sem;
    if ((sem = getSemaphore(id)) == NULL) {
        return -1;
    }

    acquire(&(sem->lock));
    if (sem->value > 0) {
        sem->value--;
        release(&(sem->lock));
    } else {
        int currentPID = getProcessPID();
        sem->blockedProcesses[sem->blockedProcessesAmount++] = currentPID;
        release(&(sem->lock));
        blockProcess(currentPID);//TODO blockear el processo desde el sched
    }
    return 0;
}

int semPost(uint32_t id) {
    semaphore *sem;
    if ((sem = getSemaphore(id)) == NULL) {
        return -1;
    }

    acquire(&(sem->lock));
    if (sem->blockedProcessesAmount > 0) {
        unblockSemProcess(sem);
    } else {
        sem->value++;
    }

    release(&(sem->lock));
    return 0;
}

int semClose(uint32_t id) {
    semaphore *sem;
    if ((sem = getSemaphore(id)) == NULL) {
        return -1;
    }

    if (sem->listeningProcesses > 0) {
        sem->listeningProcesses--;
    }

    if (sem->listeningProcesses == 0) {
        removeSemaphore(sem);
    }

    return 0;
}

void semStatus() {
    printf("\n Status of active semaphores: \n\n");
    semaphore *sem = semaphores;
    while (sem) {
        printf("    ID: %d\n", sem->id);
        printf("    Value: %d\n", sem->value);
        printf("    N° of Attached processes: %d\n", sem->listeningProcesses);
        printf("    N° of Blocked processes: %d\n", sem->blockedProcessesAmount);
        printf("    Blocked processes:\n");
        blockedProcessesDump(sem->blockedProcesses, sem->blockedProcessesAmount);
        sem = sem->next;
    }
}

static void blockedProcessesDump(int *blockedProcesses,
                                 uint16_t blockedProcessesAmount) {
    for (int i = 0; i < blockedProcessesAmount; i++) {
        printf("         PID: %d\n", blockedProcesses[i]);
    }
    printf("\n");
}

static void removeSemaphore(semaphore *sem) {
    semaphore * aux = semaphores;
    if (sem == aux) {
        semaphores = aux->next;
    } else {
        while (aux->next != sem) {
            aux = aux->next;
        }
        aux->next = sem->next;
    }
    free(sem);
}

static semaphore *createSemaphore(uint32_t id, uint64_t initialValue) {
    semaphore *newSem = malloc(sizeof(semaphore));
    if (newSem != NULL) {
        newSem->id = id;
        newSem->value = initialValue;
        newSem->blockedProcessesAmount = 0;
        newSem->listeningProcesses = 0;
        newSem->lock = 0;
        newSem->next = NULL;
        addSemaphoreToList(newSem);
    }
    return newSem;
}

static void addSemaphoreToList(semaphore *newSem) {
    semaphore *nextSem = semaphores;
    if (nextSem == NULL) {
        semaphores = newSem;
    } else {
        while (nextSem->next != NULL) {
            nextSem = nextSem->next;
        }
       nextSem->next = newSem;
    }
}

static semaphore *getSemaphore(uint32_t id) {
    semaphore *sem = semaphores;
    while (sem) {
        if (sem->id == id) {
            return sem;
        }
        sem = sem->next;
    }
    return NULL;
}

static void unblockSemProcess(semaphore *sem) {
    int PID = sem->blockedProcesses[0];
    for (int i = 0; i < sem->blockedProcessesAmount - 1; i++) {
        sem->blockedProcesses[i] = sem->blockedProcesses[i + 1];
    }
    sem->blockedProcessesAmount--;
    resumeProcess(PID); //TODO reanudar el processo en el sched
}
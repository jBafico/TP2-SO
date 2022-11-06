#ifndef SAMPLECODEMODULE_SYSCALLS_H
#define SAMPLECODEMODULE_SYSCALLS_H

#include <clock.h>
#include <stdint.h>
#include <commands.h>

#define MAX_BLOCKED_PROCESSES 15

typedef enum {READY, BLOCKED, TERMINATED} pState;

int sysWrite(const char *c, uint64_t length);
int sysRead(const char *c, uint64_t length);
void sysTime(clock * cptr);
int sysInfoReg(uint64_t * regs);
int sysPrintMem(uint8_t * mem, uint64_t address);
void sysSleep(uint64_t secs);
void sysClearScreen();

void * sysMalloc(uint64_t nBytes);
void sysFree(void * block);
int sysAddProcess(void (*entryPoint)(int, char**), int argc, char** argv, int foreground, int * fd);
void sysWait(int pid);
int sysKillProcess(uint64_t pid);
int sysGetPID();
int sysBlockProcess(uint64_t pid);
int sysSetState(uint64_t pid, pState newState);
void sysSetPriority(uint64_t pid, int newPriority);
int sysReadyProcess(uint64_t pid);
int sysSemWait(uint32_t id);
int sysSemClose(uint32_t id);
int sysSemOpen(uint32_t id, uint64_t initialValue);
int sysSemPost(uint32_t id);
void sysSemStatus();
int sysPipeOpen(int pipeId);
int sysPipeClose(int pipeId);
int sysPipeRead(int pipeId);
int sysPipeWrite( int pipeId, char * src);
void sysYield();
int sysGetProcessList(processInfo * processStruct);

//estructuras para intercambio de informacion

typedef struct info {
    uint64_t availableBytes;
    uint64_t allocatedBytes;
    uint64_t totalMemory;
} memInfo;


typedef struct semaphore {
    uint32_t id;
    uint64_t value;
    int blockedProcesses[MAX_BLOCKED_PROCESSES];
    uint16_t blockedProcessesAmount;
    uint16_t listeningProcesses;
} semaphoreData;

int sysMemInfo(memInfo * mem);
int sysSemaphoreInfo(semaphoreData * s);


#endif //SAMPLECODEMODULE_SYSCALLS_H

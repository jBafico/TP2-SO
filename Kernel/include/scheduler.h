#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {READY, BLOCKED, TERMINATED} pState;

#define MAX_NAME_LEN 40

#define SIZE_OF_STACK (4 * 1024)
#define MAX_PRIORITY 40
#define BACKGROUND_PRIORITY_DEFAULT 1
#define FOREGROUND_PRIORITY_DEFAULT 2

#define EXCHANGEBUFFERSIZE 128
#define SMALLBUFFER 64


typedef struct processInfo{
    uint64_t pid;
    char name[EXCHANGEBUFFERSIZE];
    uint64_t stackPointer;
    uint64_t basePointer;
    char type[SMALLBUFFER];
    char state[SMALLBUFFER];
} processInfo;

typedef struct process{
    int pid;
    int ppid;
    bool foreground;
    pState state;
    uint64_t priority;
    char name[MAX_NAME_LEN];
    int fds[2];
    void * stackPointer;
    void * basePointer;
    int argc;
    char ** argv;
} process;

typedef struct processNode {
    process proc;
    struct processNode *next;
} processNode;

typedef struct processList {
    uint32_t size;
    uint32_t readySize;
    processNode *first;
    processNode *last;
} processList;

void * schedule(void *sp);
void initializeScheduler();
int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int foreground, int *fd);
void wait(int pid);
int currentProcessIsForeground();
int getProcessPID();
int getCurrentProcessOutputFD();
int getCurrentProcessInputFD();
int killProcess(uint64_t pid);
void killCurrentFGProcess();
int blockProcess(uint64_t pid);
int readyProcess(uint64_t pid);
void yield();
int setState(uint64_t pid, pState newState);
void setPriority(uint64_t pid, int newPriority);
int getProcessList(processInfo * ps);
#endif //SCHEDULER_H

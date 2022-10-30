#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_NAME_LEN 32

typedef enum {READY, BLOCKED, TERMINATED} pState;

typedef struct process{
    int pid;                   /* ID of the process */
    int ppid;                  /* ID of the parent process */
    int prio;                  /* process priority */
    int foreground;            /* states if it is a foreground or background process */
    pState state;              /* State of the process */
    char name[MAX_NAME_LEN];   /* Name of the process */
    void * processSP;          /* Stack Pointer of the process */
    void * processBP;          /* Base Pointer of the process */
    int FD[2];                 /* File Descriptors of the process */
    int argc;
    char **argv;
}process;

typedef struct processNode{
    process process;
    struct processNode * next;
}processNode;

typedef struct processList{
    uint32_t size;
    uint32_t readySize;
    processNode * first;
    processNode * last;
}processList;


typedef struct {
    //levantadas por popstate
    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    //levantadas por iretq
    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;

} decieveStack;

#define SIZE_OF_STACK (4 * 1024)
#define MAX_PRIORITY 40
#define BACKGROUND_PRIORITY_DEFAULT 1
#define FOREGROUND_PRIORITY_DEFAULT 2
#define BACKGROUND 0
#define FOREGROUND 1


#define STOP_FIRST 1
#define STOP_SECOND 2

#define EXIT_KEY 28 //ASCII para la tecla de ESC

typedef void (*commandPointer)(void);

void * schedule();
void initializeScheduler();
int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int foreground, int *fd);
void callTimerTick();

void wait(int pid);
int currentProcessIsForeground();
int getProcessPID();
int getCurrentProcessOutputFD();
int getCurrentProcessInputFD();
int killProcess(uint64_t pid);
void killCurrentFGProcess();
int blockProcess(uint64_t pid);
int readyProcess(uint64_t pid);
int setState(uint64_t pid, pState newState);
void setPriority(uint64_t pid, int newPriority);
#endif //SCHEDULER_H

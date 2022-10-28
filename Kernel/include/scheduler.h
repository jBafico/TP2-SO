#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_NAME_LEN 32

typedef struct process{
    int pid;                   /* ID of the process */
    int prio;                  /* process priority */
    void * function;           /* pointer to the process function */
    char name[MAX_NAME_LEN];   /* Name of the process */
    void * processSP;          /* Stack Pointer of the process */
    void * processBP;          /* Base Pointer of the process */
    int FD[2];                 /* File Descriptors of the process */
}process;

typedef struct processNode{
    process process;
    struct processNode * next;
}processNode;

typedef struct processList{
    int size;
    processNode * first;
    processNode * last;
}processList;

#define NULL (void *) 0
#define MAX_TASKS 2

#define STOP_FIRST 1
#define STOP_SECOND 2

#define EXIT_KEY 28 //ASCII para la tecla de ESC

typedef void (*commandPointer)(void);

void addTask(commandPointer function);
void removeTask(uint8_t task);
void removeCurrentTask();
void runCurrentTask();
void runTasks();
void * schedule();
#endif //SCHEDULER_H

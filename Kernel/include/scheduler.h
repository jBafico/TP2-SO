#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

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

#endif //SCHEDULER_H

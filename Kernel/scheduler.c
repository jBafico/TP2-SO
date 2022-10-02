#include "scheduler.h"
#include "keyboard.h"

static commandPointer tasks[MAX_TASKS];
static uint8_t cantTasks = 0;
static uint8_t currentTask = 0;

void addTask(commandPointer function){
    if(cantTasks >= MAX_TASKS)
        return;
    tasks[cantTasks++] = function;
}

void removeTask(uint8_t task){
    if(task<=0 || task > MAX_TASKS)
        return;
    tasks[task-1] = NULL;
}

void removeCurrentTask(){
    if(cantTasks == 0)
        return;
    tasks[currentTask] = NULL;
    cantTasks--;
}

void runCurrentTask(){
    if(tasks[currentTask] != NULL)
        tasks[currentTask](); //TODO ver si aca le pasamos parametro desde kernel o armamos dos wrappers extra en userland
}

void runTasks(){
    uint8_t exit = 0, c;
    while ((c = getCharKernel()) != EXIT_KEY){

        if(c == STOP_FIRST)
            removeTask(STOP_FIRST);
        else if(c == STOP_SECOND)
            removeTask(STOP_SECOND);

        while(currentTask < cantTasks){
            runCurrentTask();
            currentTask++;
        }

        currentTask = 0;
    }
}





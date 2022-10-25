#include "../include/scheduler.h"
#include "keyboard.h"


//
//typedef struct process{
//    int pid;                   /* ID of the process */
//    int prio;                  /* process priority */
//    void * function;           /* pointer to the process function */
//    char name[MAX_NAME_LEN];   /* Name of the process */
//    void * processSP;          /* Stack Pointer of the process */
//    void * processBP;          /* Base Pointer of the process */
//    int FD[2];                 /* File Descriptors of the process */
//}process;
//
//typedef struct processNode{
//    process process;
//    struct processNode * next;
//}processNode;
//
//typedef struct processList{
//    int size;
//    processNode * first;
//    processNode * last;
//}processList;
//
//#define NULL (void *) 0
//#define MAX_TASKS 2




void * schedule(void * rsp){

}



void addProcess()




























//static commandPointer tasks[MAX_TASKS];
//static uint8_t cantTasks = 0;
//static uint8_t currentTask = 0;
//
//void addTask(commandPointer function){
//    if(cantTasks >= MAX_TASKS)
//        return;
//    tasks[cantTasks++] = function;
//}
//
//void removeTask(uint8_t task){
//    if(task<=0 || task > MAX_TASKS)
//        return;
//    tasks[task-1] = NULL;
//}
//
//void removeCurrentTask(){
//    if(cantTasks == 0)
//        return;
//    tasks[currentTask] = NULL;
//    cantTasks--;
//}
//
//void runCurrentTask(){
//    if(tasks[currentTask] != NULL)
//        tasks[currentTask](); //TODO ver si aca le pasamos parametro desde kernel o armamos dos wrappers extra en userland
//}
//
//void runTasks(){
//    uint8_t exit = 0, c;
//    while ((c = getCharKernel()) != EXIT_KEY){
//        if(c == STOP_FIRST)
//            removeTask(STOP_FIRST);
//        else if(c == STOP_SECOND)
//            removeTask(STOP_SECOND);
//
//        while(currentTask < cantTasks){
//            runCurrentTask();
//            currentTask++;
//        }
//        currentTask = 0;
//    }
//}
//
//



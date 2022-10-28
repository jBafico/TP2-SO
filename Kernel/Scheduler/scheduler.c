#include "../include/scheduler.h"
#include "../include/keyboard.h"
#include "../include/memManager.h"

static uint64_t currentPID = 0;
static uint64_t cyclesLeft;

static processList *processes;
static processNode *currentProcess = NULL;
static processNode *baseProcess;        //This is the process that will be running when there are no task to be executed


static int queueIsEmpty(processList *processes);
static processNode *dequeueProcess(processList *processes);
static void queueProcess(processList *processes, processNode *process);
static int initializeProcessControlBlock(process *process, char *name,
                                         uint8_t foreground, int *fd);
static uint64_t getPID();
static int getArguments(char **to, char **from, int count);

        static void freeProcess(processNode *p);
static processNode *getProcess(uint64_t pid);


void initializeScheduler() {
    processes = malloc(sizeof(processList));
    if (processes == NULL) {
        return;
    }

    processes->first = NULL;
    processes->last = processes->first;
    processes->readySize = 0;
    processes->size = 0;

    /*char *argv[] = {"Initial BASE Process"};

   newProcess(&baseProcess, 1, argv, BACKGROUND, 0); TODO setear el base process

    baseProcess = dequeueProcess(processes);
     */
}

void * schedule(void * rsp){
    if (currentProcess != NULL) {
        if (currentProcess->process.state == READY && cyclesLeft > 0) {
            cyclesLeft--;
            return rsp;
        }

        currentProcess->process.processSP = rsp;

        if (currentProcess->process.pid != baseProcess->process.pid) {
            if (currentProcess->process.state == TERMINATED) {
                processNode * parent = getProcess(currentProcess->process.ppid);
                if (parent != NULL && currentProcess->process.foreground &&
                    parent->process.state == BLOCKED) {
                    readyProcess(parent->process.pid);
                }
                freeProcess(currentProcess);
            } else {
                queueProcess(processes, currentProcess);
            }
        }
    }

    if (processes->readySize > 0) {
        currentProcess = dequeueProcess(processes);
        while (currentProcess->process.state != READY) {
            if (currentProcess->process.state == TERMINATED) {
                freeProcess(currentProcess);
            }
            if (currentProcess->process.state == BLOCKED) {
                queueProcess(processes, currentProcess);
            }
            currentProcess = dequeueProcess(processes);
        }
    } else {
        currentProcess = baseProcess;
    }

    cyclesLeft = currentProcess->process.prio;

    return currentProcess->process.processSP;
}



int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int foreground, int *fd){
    if (entryPoint == NULL) {
        return -1;
    }

    processNode *newProcess = malloc(sizeof(processNode));
    if (newProcess == NULL) {
        return -1;
    }

    if (initializeProcessControlBlock(&newProcess->process, argv[0], foreground,
                                      fd) == -1) {
        free(newProcess);
        return -1;
    }

    char **arguments = malloc(sizeof(char *) * argc);
    if (arguments == NULL) {
        free(newProcess);
        return -1;
    }

    if (getArguments(arguments, argv, argc) == -1) {
        free(newProcess);
        free(arguments);
        return -1;
    }

    newProcess->process.argc = argc;
    newProcess->process.argv = arguments;

    //TODO initialice stack frame for the new process

    newProcess->process.state = READY;

    queueProcess(processes, newProcess);
    if (newProcess->process.foreground && newProcess->process.ppid) {
        blockProcess(newProcess->process.ppid);
    }

    return newProcess->process.pid;

}

int readyProcess(uint64_t pid) {
    return setState(pid, READY);
}

int blockProcess(uint64_t pid) {
    int resPID = setState(pid, BLOCKED);

    if (pid == currentProcess->process.pid) {
        callTimerTick();
    }

    return resPID;
}

int killProcess(uint64_t pid) {
    int resPID = setState(pid, TERMINATED);

    if (pid == currentProcess->process.pid) {
        callTimerTick();
    }

    return resPID;
}

void killCurrentFGProcess() {
    if (currentProcess != NULL && currentProcess->process.foreground &&
        currentProcess->process.state == READY) {
        killProcess(currentProcess->process.pid);
    }
}

void wait(int pid) {
    processNode *process = getProcess(pid);
    if (process) {
        process->process.foreground = 1;
        blockProcess(currentProcess->process.pid);
    }
}

// INITIALIZERS
static int initializeProcessControlBlock(process *process, char *name,
                                         uint8_t foreground, int *fd) {
    strcpy(process->name, name);//TODO have to cpy name
    process->pid = getPID();
    process->ppid = (currentProcess == NULL ? 0 : currentProcess->process.pid);
    if (foreground > 1) {
        return -1;
    }

    process->foreground = (currentProcess == NULL
                       ? foreground
                       : (currentProcess->process.foreground ? foreground : 0));
    process->processBP = malloc(SIZE_OF_STACK);
    process->prio = process->foreground ? FOREGROUND_PRIORITY_DEFAULT
                                    : BACKGROUND_PRIORITY_DEFAULT;
    process->FD[0] = (fd ? fd[0] : 0);
    process->FD[1] = (fd ? fd[1] : 1);

    if (process->processBP == NULL) {
        return -1;
    }

    process->processBP = (void *)((char *)process->processBP + SIZE_OF_STACK - 1);
    process->processSP = ;//TODO GET THE STACK POINTER
    return 0;
}

// GETTERS

static int getArguments(char **to, char **from, int count) {
    for (int i = 0; i < count; i++) {
        to[i] = malloc(sizeof(char) * (strlen(from[i]) + 1));
        if (to[i] == NULL) {
            i--;
            while (i >= 0) {
                free(to[i]);
                i--;
            }
            return -1;
        }
        strcpy(to[i], from[i]);
    }
    return 0;
}

static uint64_t getPID() {
    return currentPID++;
}
int getProcessPID() {
    return currentProcess ? currentProcess->process.pid : -1;
}
int getCurrentProcessInputFD() {
    if (currentProcess) {
        return currentProcess->process.FD[0];
    } else {
        return -1;
    }
}

int getCurrentProcessOutputFD() {
    if (currentProcess) {
        return currentProcess->process.FD[1];
    } else {
        return -1;
    }
}

int currentProcessIsForeground() {
    if (currentProcess) {
        return currentProcess->process.foreground;
    } else {
        return -1;
    }
}
// SETTERS
int setState(uint64_t pid, pState newState) {
    processNode *process = getProcess(pid);
    if (process == NULL || process->process.state == TERMINATED) {
        return -1;
    }

    if (process == currentProcess) {
        process->process.state = newState;
        return process->process.pid;
    }

    if (process->process.state != READY && newState == READY) {
        processes->readySize++;
    }

    if (process->process.state == READY && newState != READY) {
        processes->readySize--;
    }

    process->process.state = newState;

    return process->process.pid;
}

void setPriority(uint64_t pid, int newPriority) {
    if (newPriority < 0) {
        newPriority = 0;
    }
    if (newPriority > MAX_PRIORITY) {
        newPriority = MAX_PRIORITY;
    }

    processNode *p = getProcess(pid);

    if (p != NULL) {
        p->process.prio = newPriority;
    }
}
// FIND FUNCITONS
static processNode *getProcess(uint64_t pid) {
    if (currentProcess != NULL && currentProcess->process.pid == pid) {
        return currentProcess;
    }

    processNode *process = processes->first;
    while (process != NULL) {
        if (process->process.pid == pid) {
            return process;
        }
        process = (processNode *)process->next;
    }
    return NULL;
}
// MEM FUNCITONS
static void freeProcess(processNode *p) {
    //TODO FREE PROCESS
}
// QUEUE FUNCITONS
void queueProcess(processList *processes, processNode *process) {
    if (queueIsEmpty(processes)) {
        processes->first = process;
        processes->last = processes->first;
    } else {
        processes->last->next = process;
        process->next = NULL;
        processes->last = process;
    }

    if (process->process.state == READY) {
        processes->readySize++;
    }

    processes->size++;
}

processNode *dequeueProcess(processList *processes) {
    if (queueIsEmpty(processes)) {
        return NULL;
    }

    processNode *first = processes->first;
    processes->first = processes->first->next;
    processes->size--;

    if (first->process.state == READY) {
        processes->readySize--;
    }

    return first;
}

int queueIsEmpty(processList *processes) {
    return processes->size == 0;
}




























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



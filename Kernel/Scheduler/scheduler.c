// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>
#include <memManager.h>
#include <interrupts.h>
#include <lib.h>
#include <naiveConsole.h>

#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))

static void init(int argc, char **argv);
static int initializeProcessControlBlock(process *proc, char *name, uint8_t foreground, const int *fd);
static int getArguments(char **to, char **from, int count);
static void beginProcessHandler(void (*entryPoint)(int, char **), int argc, char **argv);
static void initalizeDecieveStack(void (*entryPoint)(int, char **), int argc, char **argv, void *stackPointer);
static void freeProcess(processNode *p);
static uint64_t getPID();
static void kill();
static processNode *getProcess(uint64_t pid);

static uint64_t currentPID = 0;
static uint64_t cyclesLeft;

static processList *processes;
static processNode *currentProcess = NULL;
static processNode *baseProcess; //This is the process that will be running when there are no task to be executed

int queueIsEmpty(processList *prcs) { return prcs->size == 0; }

void registerMemory(void * dir){
    currentProcess->proc.allocDir[currentProcess->proc.size++] = dir;
}

void queueProcess(processList *prcs, processNode *process) {
    if (queueIsEmpty(prcs)) {
        prcs->first = process;
        prcs->last = prcs->first;
    } else {
        prcs->last->next = process;
        process->next = NULL;
        prcs->last = process;
    }

    if (process->proc.state == READY) {
        prcs->readySize++;
    }

    prcs->size++;
}

processNode *dequeueProcess(processList *prcs) {
    if (queueIsEmpty(prcs))
        return NULL;

    processNode *first = prcs->first;
    prcs->first = prcs->first->next;
    prcs->size--;

    if (first->proc.state == READY)
        prcs->readySize--;

    return first;
}

void initializeScheduler() {
    processes = malloc(sizeof(processList));

    if (processes == NULL)
        return;

    processes->first = NULL;
    processes->last = processes->first;
    processes->readySize = 0;
    processes->size = 0;

    char *argv[] = {"Init"};

    addProcess(&init, 1, argv, false, NULL);

    baseProcess = dequeueProcess(processes);
}


void * schedule(void *stackPointer) {
    if (currentProcess != NULL) {
        if (currentProcess->proc.state == READY && cyclesLeft > 0) {
            cyclesLeft--;
            return stackPointer;
        }

        currentProcess->proc.stackPointer = stackPointer;

        if (currentProcess->proc.pid != baseProcess->proc.pid) {
            if (currentProcess->proc.state == TERMINATED) {
                processNode *parent = getProcess(currentProcess->proc.ppid);
                if (parent != NULL && currentProcess->proc.foreground &&
                    parent->proc.state == BLOCKED) {
                    readyProcess(parent->proc.pid);
                }
                freeProcess(currentProcess);
            }
            else
                queueProcess(processes, currentProcess);
        }
    }

    if (processes->readySize > 0) {
        currentProcess = dequeueProcess(processes);
        if(currentProcess != NULL){
            while (currentProcess->proc.state != READY) {
                if (currentProcess->proc.state == TERMINATED)
                    freeProcess(currentProcess);

                if (currentProcess->proc.state == BLOCKED)
                    queueProcess(processes, currentProcess);

                currentProcess = dequeueProcess(processes);
            }
        }
    }
    else
        currentProcess = baseProcess;

    cyclesLeft = currentProcess->proc.priority;

    return currentProcess->proc.stackPointer;
}

int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int foreground, int *fd) {
    if (entryPoint == NULL)
        return ERROR;

    processNode *newProcess = malloc(sizeof(processNode));
    if (newProcess == NULL)
        return ERROR;

    if (initializeProcessControlBlock(&newProcess->proc, argv[0], foreground, fd) == ERROR) {
        free(newProcess);
        return ERROR;
    }

    char **arguments = malloc(sizeof(char *) * argc);
    if (arguments == NULL) {
        free(newProcess);
        return ERROR;
    }

    if (getArguments(arguments, argv, argc) == ERROR) {
        free(newProcess);
        free(arguments);
        return ERROR;
    }

    newProcess->proc.argc = argc;
    newProcess->proc.argv = arguments;

    initalizeDecieveStack(entryPoint, argc, arguments, newProcess->proc.stackPointer);

    newProcess->proc.state = READY;

    queueProcess(processes, newProcess);
    if (newProcess->proc.foreground && newProcess->proc.ppid)
        blockProcess(newProcess->proc.ppid);

    return newProcess->proc.pid;
}



int killProcess(uint64_t pid) {
    //Shell and base process cannot be killed
    if(pid <= 2)
        return ERROR;


    //releaseMem(pid)
    int resPID = setState(pid, TERMINATED);

    if (pid == currentProcess->proc.pid)
        forceTimerTick();

    return resPID;
}

int blockProcess(uint64_t pid) {
    int resPID = setState(pid, BLOCKED);

    if (pid == currentProcess->proc.pid)
        forceTimerTick();

    return resPID;
}

int readyProcess(uint64_t pid) { return setState(pid, READY); }

int getProcessPID() { return currentProcess ? currentProcess->proc.pid : ERROR; }

void yield() {
    cyclesLeft = 0;
    forceTimerTick();
}

int setState(uint64_t pid, pState newState) {
    processNode *process = getProcess(pid);

    if (process == NULL || process->proc.state == TERMINATED)
        return ERROR;

    if (process == currentProcess) {
        process->proc.state = newState;
        return process->proc.pid;
    }

    if (process->proc.state != READY && newState == READY)
        processes->readySize++;

    if (process->proc.state == READY && newState != READY)
        processes->readySize--;

    process->proc.state = newState;

    return process->proc.pid;
}

void setPriority(uint64_t pid, int newPriority) {
    if (newPriority < 0)
        newPriority = 0;

    if (newPriority > MAX_PRIORITY)
        newPriority = MAX_PRIORITY;


    processNode *p = getProcess(pid);

    if (p != NULL)
        p->proc.priority = newPriority;
}

void killCurrentFGProcess() {
    if (currentProcess != NULL && currentProcess->proc.foreground &&
        currentProcess->proc.state == READY) {
        killProcess(currentProcess->proc.pid);
    }
}

int getCurrentProcessInputFD() {
    if (currentProcess)
        return currentProcess->proc.fds[0];
    else
        return ERROR;
}

int getCurrentProcessOutputFD() {
    if (currentProcess)
        return currentProcess->proc.fds[1];
    else
        return ERROR;
}

void wait(int pid) {
    processNode *process = getProcess(pid);
    if (process) {
        process->proc.foreground = 1;
        blockProcess(currentProcess->proc.pid);
    }
}

static void init(int argc, char **argv) {
    while (1) {
        _hlt();
    }
}

static uint64_t getPID() { return currentPID++; }

static int initializeProcessControlBlock(process *proc, char *name, uint8_t foreground, const int *fd) {
    strcpy(proc->name, name);
    proc->pid = getPID();
    proc->size = 0;
    proc->ppid = (currentProcess == NULL ? 0 : currentProcess->proc.pid);
    if (foreground > 1) {
        return ERROR;
    }

    proc->foreground = (currentProcess == NULL
                        ? foreground
                        : (currentProcess->proc.foreground ? foreground : 0));
    proc->basePointer = malloc(SIZE_OF_STACK);
    proc->returnDir = proc->basePointer;
    proc->priority = proc->foreground ? FOREGROUND_PRIORITY_DEFAULT
                                      : BACKGROUND_PRIORITY_DEFAULT;
    proc->fds[0] = (fd ? fd[0] : 0);
    proc->fds[1] = (fd ? fd[1] : 1);

    if (proc->basePointer == NULL) {
        return ERROR;
    }

    proc->basePointer = (void *)( __ALIGN_MASK( (uint64_t)proc->basePointer + SIZE_OF_STACK - 1, 0X07));
    proc->stackPointer = (void *)((char*)proc->basePointer - sizeof(decieveStack));
    return 0;
}

static int getArguments(char **to, char **from, int count) {
    for (int i = 0; i < count; i++) {
        to[i] = malloc(sizeof(char) * (strlen(from[i]) + 1));
        if (to[i] == NULL) {
            i--;
            while (i >= 0) {
                free(to[i]);
                i--;
            }
            return ERROR;
        }
        strcpy(to[i], from[i]);
    }
    return 0;
}

void kill() {
    killProcess(currentProcess->proc.pid);
    forceTimerTick();
}

void beginProcessHandler(void (*entryPoint)(int, char **), int argc, char **argv) {
    entryPoint(argc, argv);
    kill();
}

static void initalizeDecieveStack(void (*entryPoint)(int, char **), int argc, char **argv, void *stackPointer) {
    decieveStack *fakeStack = (decieveStack *)stackPointer;

    fakeStack->gs = 1;
    fakeStack->fs = 1;
    fakeStack->r15 = 1;
    fakeStack->r14 = 1;
    fakeStack->r13 = 1;
    fakeStack->r12 = 1;
    fakeStack->r11 = 1;
    fakeStack->r10 = 1;
    fakeStack->r9 = 1;
    fakeStack->r8 = 1;
    fakeStack->rsi = (uint64_t)argc;
    fakeStack->rdi = (uint64_t)entryPoint;
    fakeStack->basePointer = 0x00B;
    fakeStack->rdx = (uint64_t)argv;
    fakeStack->rcx = 1;
    fakeStack->rbx = 1;
    fakeStack->rax = 1;
    fakeStack->rip = (uint64_t)beginProcessHandler;
    fakeStack->cs = 0x008;
    fakeStack->eflags = 0x202;
    fakeStack->stackPointer = (uint64_t)(fakeStack);
    fakeStack->ss = 0x000;
    fakeStack->base = 0x000;
}

static processNode *getProcess(uint64_t pid) {
    if (currentProcess != NULL && currentProcess->proc.pid == pid) {
        return currentProcess;
    }

    processNode *process = processes->first;
    while (process != NULL) {
        if (process->proc.pid == pid) {
            return process;
        }
        process = (processNode *)process->next;
    }
    return NULL;
}

static void freeProcess(processNode *p) {
    if(p != NULL){
        for (int i = 0; i < p->proc.argc; i++) {
            free(p->proc.argv[i]);
        }
        for (int i = 0; i < p->proc.size; ++i) {
            free(p->proc.allocDir[i]);
        }
        free(p->proc.returnDir);
        free(p->proc.argv);
        free((void *)p);
    }
}




int getProcessList(processInfo * ps) {
    processNode *current = processes->first;
    int i;
    for (i = 0; i < processes->size; i++, current = current->next) {
        strcpy(ps[i].name, current->proc.name);
        strcpy(ps[i].type, current->proc.foreground ? "Foreground" : "Background");
        strcpy(ps[i].state, current->proc.state == READY ? "Ready" : current->proc.state == BLOCKED ? "Blocked"
                                                                                                    : "Terminated");
        ps[i].pid = current->proc.pid;
        ps[i].basePointer = (uint64_t) current->proc.basePointer;
        ps[i].stackPointer = (uint64_t) current->proc.stackPointer;
    }
    //agregamos el current process
    strcpy(ps[i].name, currentProcess->proc.name);
    strcpy(ps[i].type, currentProcess->proc.foreground ? "Foreground" : "Background");
    strcpy(ps[i].state, "Running");
    ps[i].pid = currentProcess->proc.pid;
    ps[i].basePointer = (uint64_t) currentProcess->proc.basePointer;
    ps[i].stackPointer = (uint64_t) currentProcess->proc.stackPointer;
    i++;
    return i;
}

void getProcessFDS(int * fds){
    fds[0]=currentProcess->proc.fds[0];
    fds[1]=currentProcess->proc.fds[1];
}
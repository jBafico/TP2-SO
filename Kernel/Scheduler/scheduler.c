#include <scheduler.h>
#include <memManager.h>
#include <interrupts.h>
#include <lib.h>
#include <naiveConsole.h>

typedef struct {
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
    uint64_t basePointer;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t stackPointer;
    uint64_t ss;
    uint64_t base;
} fakeStack;

static void init(int argc, char **argv);
static int initializeProcessControlBlock(process *proc, char *name, uint8_t foreground, const int *fd);
static int getArguments(char **to, char **from, int count);
static void beginProcessHandler(void (*entryPoint)(int, char **), int argc, char **argv);
static void initializeProcessStackFrame(void (*entryPoint)(int, char **), int argc, char **argv, void *basePointer);
static void freeProcess(processNode *p);
static uint64_t getPID();
static void kill();
static processNode *getProcess(uint64_t pid);

static uint64_t currentPID = 0;
static uint64_t cyclesLeft;

static processList *processes;
static processNode *currentProcess = NULL;
static processNode *baseProcess;

int queueIsEmpty(processList *prs) { return prs->size == 0; }

void queueProcess(processList *prs, processNode *process) {
    if (queueIsEmpty(prs)) {
        prs->first = process;
        prs->last = prs->first;
    } else {
        prs->last->next = process;
        process->next = NULL;
        prs->last = process;
    }

    if (process->proc.state == READY) {
        prs->readySize++;
    }

    prs->size++;
}

processNode *dequeueProcess(processList *prs) {
    if (queueIsEmpty(prs))
        return NULL;

    processNode *first = prs->first;
    prs->first = prs->first->next;
    prs->size--;

    if (first->proc.state == READY)
        prs->readySize--;

    return first;
}

void initializeScheduler() {
    processes = malloc(sizeof(processList));
    if (processes == NULL) {
        return;
    }

    processes->first = NULL;
    processes->last = processes->first;
    processes->readySize = 0;
    processes->size = 0;

    char *argv[] = {"Init Base Process"};

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
        while (currentProcess->proc.state != READY) {
            if (currentProcess->proc.state == TERMINATED)
                freeProcess(currentProcess);

            if (currentProcess->proc.state == BLOCKED)
                queueProcess(processes, currentProcess);

            currentProcess = dequeueProcess(processes);
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

    initializeProcessStackFrame(entryPoint, argc, arguments, newProcess->proc.basePointer);

    newProcess->proc.state = READY;

    queueProcess(processes, newProcess);
    if (newProcess->proc.foreground && newProcess->proc.ppid) {
        blockProcess(newProcess->proc.ppid);
    }

    return newProcess->proc.pid;
}

int killProcess(uint64_t pid) {
    int resPID = setState(pid, TERMINATED);

    if (pid == currentProcess->proc.pid) {
        forceTimerTick();
    }

    return resPID;
}

int blockProcess(uint64_t pid) {
    int resPID = setState(pid, BLOCKED);

    if (pid == currentProcess->proc.pid) {
        forceTimerTick();
    }

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
    if (process == NULL || process->proc.state == TERMINATED) {
        return ERROR;
    }

    if (process == currentProcess) {
        process->proc.state = newState;
        return process->proc.pid;
    }

    if (process->proc.state != READY && newState == READY) {
        processes->readySize++;
    }

    if (process->proc.state == READY && newState != READY) {
        processes->readySize--;
    }

    process->proc.state = newState;

    return process->proc.pid;
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
        p->proc.priority = newPriority;
    }
}

void killCurrentFGProcess() {
    if (currentProcess != NULL && currentProcess->proc.foreground &&
        currentProcess->proc.state == READY) {
        killProcess(currentProcess->proc.pid);
    }
}

int currentProcessIsForeground() {
    if (currentProcess) {
        return currentProcess->proc.foreground;
    } else {
        return ERROR;
    }
}

int getCurrentProcessInputFD() {
    if (currentProcess) {
        return currentProcess->proc.fds[0];
    } else {
        return ERROR;
    }
}

int getCurrentProcessOutputFD() {
    if (currentProcess) {
        return currentProcess->proc.fds[1];
    } else {
        return ERROR;
    }
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
    proc->ppid = (currentProcess == NULL ? 0 : currentProcess->proc.pid);
    if (foreground > 1) {
        return ERROR;
    }

    proc->foreground = (currentProcess == NULL
                       ? foreground
                       : (currentProcess->proc.foreground ? foreground : 0));
    proc->basePointer = malloc(SIZE_OF_STACK);
    proc->priority = proc->foreground ? FOREGROUND_PRIORITY_DEFAULT
                                    : BACKGROUND_PRIORITY_DEFAULT;
    proc->fds[0] = (fd ? fd[0] : 0);
    proc->fds[1] = (fd ? fd[1] : 1);

    if (proc->basePointer == NULL) {
        return ERROR;
    }

    proc->basePointer = (void *)((char *)proc->basePointer + SIZE_OF_STACK - 1);
    proc->stackPointer = (void *)((fakeStack *)proc->basePointer - 1);
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

static void kill() {
    (void)killProcess(currentProcess->proc.pid);
    forceTimerTick();
}

static void beginProcessHandler(void (*entryPoint)(int, char **), int argc, char **argv) {
    entryPoint(argc, argv);
    kill();
}

static void initializeProcessStackFrame(void (*entryPoint)(int, char **),
                                        int argc, char **argv, void *basePointer) {
    fakeStack *stackFrame = (fakeStack *)basePointer - 1;

    stackFrame->gs = 0x001;
    stackFrame->fs = 0x002;
    stackFrame->r15 = 0x003;
    stackFrame->r14 = 0x004;
    stackFrame->r13 = 0x005;
    stackFrame->r12 = 0x006;
    stackFrame->r11 = 0x007;
    stackFrame->r10 = 0x008;
    stackFrame->r9 = 0x009;
    stackFrame->r8 = 0x00A;
    stackFrame->rsi = (uint64_t)argc;
    stackFrame->rdi = (uint64_t)entryPoint;
    stackFrame->basePointer = 0x00B;
    stackFrame->rdx = (uint64_t)argv;
    stackFrame->rcx = 0x00C;
    stackFrame->rbx = 0x00D;
    stackFrame->rax = 0x00E;
    stackFrame->rip = (uint64_t)beginProcessHandler;
    stackFrame->cs = 0x008;
    stackFrame->eflags = 0x202;
    stackFrame->stackPointer = (uint64_t)(&stackFrame->base);
    stackFrame->ss = 0x000;
    stackFrame->base = 0x000;
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
    for (int i = 0; i < p->proc.argc; i++) {
        free(p->proc.argv[i]);
    }
    free(p->proc.argv);
    free((void *)((char *)p->proc.basePointer - SIZE_OF_STACK + 1));
    free((void *)p);
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
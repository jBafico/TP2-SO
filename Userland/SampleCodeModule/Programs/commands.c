// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <sysCalls.h>
#include <library.h>
#include <semTest.h>

#define MAXPROCESSES 20
#define FIRSTARG 1
#define SECONDARG 2
#define MAXSEM 25
#define MAXPIPE 25

#define SECS_LOOP 2

#define QTY_LOOPS "10"

void ps(int argc, char ** argv){
    processInfo processes[MAXPROCESSES];
    int processesRecieved = sysGetProcessList(processes);

    printk("Process information\n");
    for ( int i = 0; i < processesRecieved ; i++) {
        processInfo current = processes[i];
        printk("PID: %d | Name: %s | SP: %x | BP: %x | Type: %s | State: %s\n", current.pid, current.name, current.stackPointer, current.basePointer, current.type, current.state);
    }
}

void mem(int argc, char ** argv){
    memInfo mem;
    sysMemInfo(&mem);
    printk("MEMORY DATA:\n Allocated Bytes: %d | Available Bytes: %d | Total Memory: %d\n",mem.allocatedBytes,mem.availableBytes,mem.totalMemory);
}

void loop(int argc, char ** argv){
    sysMalloc(sizeof(int) * 10000);
//    int pid = sysGetPID();
//    while (1){
//        printk("Hey! I'm process %d. See you again in %d seconds, handsome ;)\n", pid, SECS_LOOP);
//        sysSleep(SECS_LOOP);
//    }
}

void nice(int argc, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    int priority = myAtoi(argv[SECONDARG]);
    sysSetPriority(pid,priority);
}

void block(int argc, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    sysBlockProcess(pid);
}

void unblock(int argc, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    sysReadyProcess(pid);
}

void kill(int argc, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    if(sysKillProcess(pid) == ERROR)
        printErr("Process not found or can't be killed\n");
}

void pipeInfo(int argc, char ** argv){
    pipeData pipes[MAXPIPE];
    int activePipes= sysPipeInfo(pipes);

    if(activePipes == 0)
        printk("There are no pipes\n");

    for (int i = 0; i < activePipes; i++){
        pipeData current = pipes[i];
        printk("Pipe ID: %d | Read Status: %s | Write Status: %s | Processes conected: %d\n",
        current.id, current.readState, current.writeState, current.processesUsing);
    }
}

void semInfo(int argc, char ** argv){
    semaphoreData semaphores[MAXSEM];
    int activeSems = sysSemaphoreInfo(semaphores);

    if(activeSems == 0)
        printk("No active semaphores\n");

    for (int i = 0; i < activeSems ; i++){
        semaphoreData current = semaphores[i];
        printk("Sem ID: %d | Value: %d | Qty. blocked processes: %d | Qty. listening processes: %d\n",
        current.id,current.value,(int)current.blockedProcessesAmount,(int)current.listeningProcesses);

        if(current.blockedProcessesAmount > 0){
            printk("Blocked processes: ");
            for ( int j = 0; j < current.blockedProcessesAmount ; j++){
                printk("%d | ",current.blockedProcesses[j]);
            }
        }
        printk("\n");
    }
}

void cat(int argc, char ** argv){
    char c = 0;
    while ((c = getChar()) != EOF) {
        if(c != ERROR)
            putCharacter(STDOUT, c);
    }
    putCharacter(STDOUT, '\n');
}

void wc(int argc, char ** argv){
    char c;
    int lines = 1;
    while ((c = getChar()) != EOF) {
        if(c != ERROR){
            putCharacter(STDOUT, c);
            if (c == '\n')
                lines++;
        }
    }
    printk("\nCantidad de lineas: %d\n", lines);
}

void filter(int argc, char ** argv){
    char c;
    while ((c = getChar()) != EOF) {
        if(c != ERROR && !isVowel(c)){
            putCharacter(STDOUT, c);
        }
    }
    putCharacter(STDOUT, '\n');
}

void semTest(int argc, char ** argv){
    char * argvv[] = {QTY_LOOPS, "1"};
    semSyncTest(2, argvv);
}


void semNoSyncTest(int argc, char ** argv){
    char * argvv[] = {QTY_LOOPS, "0"};
    semSyncTest(2, argvv);
}


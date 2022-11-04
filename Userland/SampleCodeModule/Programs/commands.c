#include <commands.h>
#include <sysCalls.h>
#include <library.h>

#define MAXPROCESSES 20
#define FIRSTARG 1
#define SECONDARG 2
#define MAXSEM 25

#define SECS_LOOP 2

void ps(int argc, char ** argv){
    processStruct processes[MAXPROCESSES];
    int processesRecieved = sysGetProcessList(processes);

    printk("Process information\n");
    for ( int i = 0; i < processesRecieved ; i++) {
        processStruct current = processes[i];
        printk("PID: %d | Name: %s | SP: %x | BP: %x | Type: %s | State: %s\n", current.pid, current.name, current.stackPointer, current.basePointer, current.type,
               current.state);
    }
}

void mem(int argc, char ** argv){
    memInfo mem;
    sysMemInfo(&mem);
    printk("MEMORY DATA:\n Allocated Bytes: %d | Available Bytes: %d | Total Memory: %d\n",mem.allocatedBytes,mem.availableBytes,mem.totalMemory);
}

void loop(int argc, char ** argv){
    int pid = sysGetPID();
    while (1){
        printk("Hey! I'm process %d. See you again in %d seconds, handsome ;)\n", pid, SECS_LOOP);
        sysSleep(SECS_LOOP);
    }
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

void unblock(int arg, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    sysReadyProcess(pid);
}

void kill(int arg, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    sysKillProcess(pid);
}


void semInfo(int arg, char ** argv){
    semaphoreData semaphores[MAXSEM];
    int activeSems = sysSemaphoreInfo(semaphores);
    for ( int i = 0; i < activeSems ; i++){
        semaphoreData current = semaphores[i];
        printk("sem id: %d | value: %d | blocked processes: %d | listening processes %d\n",
        current.id,current.value,(int)current.blockedProcessesAmount,(int)current.listeningProcesses);
        printk("Blocked processes:\n");
        for ( int j = 0; j < current.blockedProcessesAmount ; j++){
            printk("|%d",current.blockedProcesses[j]);
        }
        printk("\n");
    }
}


void cat(int arg, char ** argv){
    int c;
    while ((c = getChar()) != EOF) {
        putCharacter(c);
    }
}

void wc(int arg, char ** argv){
    char c;
    int lines = 1;
    while ((c = getChar()) != EOF) {
        putCharacter(c);
        if (c == '\n') {
            lines++;
        }
    }
    printk("\nCantidad de lineas: %d\n", lines);
}

void filter(int arg, char ** argv){
    char c;
    while ((c = getChar()) != EOF) {
        if (!isVowel(c)) {
            putCharacter(c);
        }
    }
}




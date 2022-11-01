#include <commands.h>
#include <sysCalls.h>
#include <library.h>

#define MAXPROCESSES 20
#define FIRSTARG 1
#define SECONDARG 2

void ps(int argc, char ** argv){
    processStruct processes[MAXPROCESSES];
    int processesRecieved = sysGetProcessList(processes);

    for ( int i = 0; i < processesRecieved ; i++) {
        processStruct current = processes[i];
        printk("%d,%s,%d,%d,%s,%s\n", current.pid, current.name, current.stackPointer, current.basePointer, current.type,
               current.state);
    }

}


void nice(int argc, char ** argv){
    int pid = myAtoi(argv[FIRSTARG]);
    int priority = myAtoi(argv[SECONDARG]);
    //TODO @JUAN validacion de prioridad
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




#include <semTest.h>
#include <sysCalls.h>
#include <utilTest.h>
#include <library.h>

#define PAIR_PROCESSES 2
#define SEM_ID 55
int64_t global;
int64_t iterations = 0;
void slowInc(int64_t *p, int64_t inc) {
    int64_t aux = *p;
    aux += inc;
    sysYield();
    *p = aux;
}

void inc(int argc, char *argv[]) {
    uint64_t sem = satoi(argv[1]);
    uint64_t value = satoi(argv[2]);
    uint64_t N = satoi(argv[3]);
    uint64_t i;
    if (sem && sysSemOpen(SEM_ID, 1) == -1) {
        printErr("ERROR OPENING SEM\n");
        return;
    }
    for (i = 0; i < N; i++) {
        if (sem && sysSemWait(SEM_ID) != 0) {
            printErr("ERROR WAITING SEM\n");
        }
        slowInc(&global, value);
        if (sem && sysSemPost(SEM_ID) != 0) {
            printErr("ERROR POSTING SEM\n");
        }
    }
    if (sem && sysSemClose(SEM_ID) != 0) {
        printErr("ERROR CLOSING SEM\n");
    }
    printk("\nFinal value: %d\n", global);
}

void semSyncTest() {
    uint64_t i;
    global = 0;
    printk("CREATING PROCESSES...(WITH SEM)\n");
    for (i = 0; i < PAIR_PROCESSES; i++) {
        char *argv1[] = {"inc process with sem", "1", "1", "10000"};
        sysAddProcess(&inc, 4, argv1, BACKGROUND, NULL);
        char *argv2[] = {"inc process with sem", "1", "-1", "10000"};
        sysAddProcess(&inc, 4, argv2, BACKGROUND, NULL);
    }
}

void semNoSyncTest() {
    uint64_t i;
    global = 0;
    printk("CREATING PROCESSES...(WITHOUT SEM)\n");
    for (i = 0; i < PAIR_PROCESSES; i++) {
        char *argv1[] = {"inc process without sem", "0", "1", "10000"};
        sysAddProcess(&inc, 4, argv1, BACKGROUND, NULL);
        char *argv2[] = {"inc process without sem", "0", "-1", "10000"};
        sysAddProcess(&inc, 4, argv2, BACKGROUND, NULL);
    }
}
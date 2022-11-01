// #include "../include/semTest.h"
// #include "../include/sysCalls.h"
// #include "../include/utilTest.h"

// #define PAIR_PROCESSES 2
// #define SEM_ID 55

// int64_t global;
// int64_t iterations = 0;


// void slowInc(int64_t *p, int64_t inc) {
//     int64_t aux = *p;
//     aux += inc;
//     sysYield(); 
//     *p = aux;
// }

// void inc(int argc, char *argv[]) {
//     uint64_t sem = strToInt(argv[1], 0);
//     uint64_t value = strToInt(argv[2], 0);
//     uint64_t N = strToInt(argv[3], 0);
//     uint64_t i;

//     if (sem && sysSemOpen(SEM_ID, 1) == -1) {
//         printf("ERROR OPENING SEM\n");// TODO cambiar a printk
//         return;
//     }

//     for (i = 0; i < N; i++) {
//         if (sem && sysSemWait(SEM_ID) != 0) {
//             printf("ERROR WAITING SEM\n"); // TODO cambiar a printk
//         }
//         slowInc(&global, value);
//         if (sem && sysSemPost(SEM_ID) != 0) {
//             printf("ERROR POSTING SEM\n"); // TODO cambiar a printk
//         }
//     }

//     if (sem && sysSemClose(SEM_ID) != 0) {
//         printf("ERROR CLOSING SEM\n"); // TODO cambiar a printk
//     }

//     printf("\nFinal value: %d\n", global); // TODO cambiar a printk

// }

// void semSyncTest() {
//     uint64_t i;

//     global = 0;

//     printf("CREATING PROCESSES...(WITH SEM)\n");// TODO cambiar a printk

//     for (i = 0; i < PAIR_PROCESSES; i++) {
//         char *argv1[] = {"inc process with sem", "1", "1", "10000"};
//         sysAddProcess(&inc, 4, argv1, BACKGROUND);
//         char *argv2[] = {"inc process with sem", "1", "-1", "10000"};
//         sysAddProcess(&inc, 4, argv2, BACKGROUND);
//     }
// }

// void semNoSyncTest() {
//     uint64_t i;

//     global = 0;

//     printf("CREATING PROCESSES...(WITHOUT SEM)\n"); // TODO cambiar a printk

//     for (i = 0; i < PAIR_PROCESSES; i++) {
//         char *argv1[] = {"inc process without sem", "0", "1", "10000"};
//         sysAddProcess(&inc, 4, argv1, BACKGROUND, NULL);
//         char *argv2[] = {"inc process without sem", "0", "-1", "10000"};
//         sysAddProcess(&inc, 4, argv2, BACKGROUND, NULL);
//     }
// }
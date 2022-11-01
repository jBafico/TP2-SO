// #include "../include/prioTest.h"
// #include "../include/utilTest.h"
// #include <stdint.h>
// #include "../include/sysCalls.h"

// #define TOTAL_PROCESSES 3

// void prioTest() {
//     uint64_t pids[TOTAL_PROCESSES];
//     uint64_t i;

//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         char *argv[] = {"Loop Process"};
//         pids[i] = sysAddProcess(&endless_loop_print, 1, argv, BACKGROUND);
//     }

//     busyWait(TOTAL_PROCESSES * WAIT);

//     print("\nCHANGING PRIORITIES...\n");//TODO cambiar a print k

//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         switch (i % 3) {
//             case 0:
//                 sysSetPriority(pids[i], 1);  // lowest priority
//                 break;
//             case 1:
//                 sysSetPriority(pids[i], 20);  // medium priority
//                 break;
//             case 2:
//                 sysSetPriority(pids[i], 40);  // highest priority
//                 break;
//         }
//     }

//     busyWait(TOTAL_PROCESSES * WAIT);

//     printf("\nBLOCKING...\n"); //TODO cambiar a printk

//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         sysBlockProcess(pids[i]);
//     }

//     printf("\nCHANGING PRIORITIES WHILE BLOCKED...\n");//TODO cambiar a printk
//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         sysSetPriority(pids[i], 20);
//     }

//     printf("\nUNBLOCKING...\n");//TODO cambiar a printk

//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         sysReadyProcess(pids[i]);
//     }

//     busyWait(TOTAL_PROCESSES * WAIT);
//     printf("\nKILLING...\n");//TODO cambiar a printk

//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         sysKillProcess(pids[i]);
//     }
// }
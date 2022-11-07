// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 #include <prioTest.h>
 #include <utilTest.h>
 #include <stdint.h>
 #include <sysCalls.h>
#include <library.h>

 #define TOTAL_PROCESSES 3

 void prioTest() {
     uint64_t pids[TOTAL_PROCESSES];
     uint64_t i;

     for (i = 0; i < TOTAL_PROCESSES; i++) {
         char *argv[] = {"Loop Process"};
         pids[i] = sysAddProcess(&endless_loop_print, 1, argv, BACKGROUND, NULL);
     }

     bussy_wait(TOTAL_PROCESSES * WAIT);

     printk("\nCHANGING PRIORITIES...\n");

     for (i = 0; i < TOTAL_PROCESSES; i++) {
         switch (i % 3) {
             case 0:
                 sysSetPriority(pids[i], 1);  // lowest priority
                 break;
             case 1:
                 sysSetPriority(pids[i], 20);  // medium priority
                 break;
             case 2:
                 sysSetPriority(pids[i], 40);  // highest priority
                 break;
         }
     }

     bussy_wait(TOTAL_PROCESSES * WAIT);

     printk("\nBLOCKING...\n");

     for (i = 0; i < TOTAL_PROCESSES; i++) {
         sysBlockProcess(pids[i]);
     }

     printk("\nCHANGING PRIORITIES WHILE BLOCKED...\n");
     for (i = 0; i < TOTAL_PROCESSES; i++) {
         sysSetPriority(pids[i], 20);
     }

     printk("\nUNBLOCKING...\n");

     for (i = 0; i < TOTAL_PROCESSES; i++) {
         sysReadyProcess(pids[i]);
     }

     bussy_wait(TOTAL_PROCESSES * WAIT);
     printk("\nKILLING...\n");

     for (i = 0; i < TOTAL_PROCESSES; i++) {
         sysKillProcess(pids[i]);
     }
 }
 #include <processTest.h>
 #include <sysCalls.h>
 #include <utilTest.h>
#include <library.h>

 #define MAX_PROCESSES 10

 enum State { RUNNING, WAITING, KILLED };

 typedef struct P_rq {
     uint32_t pid;
     enum State state;
 } p_rq;

 void processTest() {
     p_rq p_rqs[MAX_PROCESSES];
     uint8_t rq;
     uint8_t alive = 0;
     uint8_t action;

     while (1) {
         for (rq = 0; rq < MAX_PROCESSES; rq++) {
             char *argv[] = {"Loop Process"};
             p_rqs[rq].pid = sysAddProcess(&endless_loop_print, 1, argv, BACKGROUND);
             if (p_rqs[rq].pid == -1) {
                 printErr("Error creating process\n");
                 return;
             } else {
                 p_rqs[rq].state = RUNNING;
                 alive++;
             }
         }

         while (alive > 0) {
             for (rq = 0; rq < MAX_PROCESSES; rq++) {
                 action = GetUniform(2) % 2;
                 switch (action) {
                     case 0:
                         if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == WAITING) {
                             if (sysKillProcess(p_rqs[rq].pid) == -1) {
                                 printErr("Error killing process\n");
                                 return;
                             }
                             p_rqs[rq].state = KILLED;
                             alive--;
                             bussy_wait(WAIT);
                         }
                         break;

                     case 1:
                         if (p_rqs[rq].state == RUNNING) {
                             if (sysBlockProcess(p_rqs[rq].pid) == -1) {
                                 printErr("Error blocking process\n");
                                 return;
                             }
                             p_rqs[rq].state = WAITING;
                         }
                         break;
                 }
             }
             for (rq = 0; rq < MAX_PROCESSES; rq++) {
                 if (p_rqs[rq].state == WAITING && GetUniform(2) % 2) {
                     if (sysReadyProcess(p_rqs[rq].pid) == -1) {
                         printErr("Error unblocking process\n");
                         return;
                     }
                     p_rqs[rq].state = RUNNING;
                 }
             }
         }
     }
 }

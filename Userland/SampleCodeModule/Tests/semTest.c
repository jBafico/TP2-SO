// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semTest.h>
#include <sysCalls.h>
#include <utilTest.h>
#include <library.h>

#define PAIR_PROCESSES 2
#define SEM_ID 55

uint64_t global;  //shared memory

void slowInc(uint64_t *p, uint64_t inc){
  uint64_t aux = *p;
  sysYield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(int argc, char *argv[]){
  int64_t n;
  int64_t inc;
  int64_t use_sem;

  if (argc != 4) return;

  if ((n = satoi(argv[1])) <= 0) return ;
  if ((inc = satoi(argv[2])) == 0) return ;
  if ((use_sem = satoi(argv[3])) < 0) return ;


  if (use_sem)
    if (sysSemOpen(SEM_ID, 1) == ERROR){
      printk("test_sync: ERROR opening semaphore\n");
      return;
    }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) sysSemWait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem) sysSemPost(SEM_ID);
  }

  if (use_sem) sysSemClose(SEM_ID);

}

void semSyncTest(int argc, char ** argv){ //{n, use_sem}
  uint64_t pids[2 * PAIR_PROCESSES];

  char * argvDec[] = {"Decrement", argv[0], "-1", argv[1]};
  char * argvInc[] = {"Increment", argv[0], "1",argv[1]};

  global = 0;
  uint64_t i;
  for(i = 0; i < PAIR_PROCESSES; i++){
    pids[i] = sysAddProcess(my_process_inc, 4, argvDec,false,NULL);
    pids[i + PAIR_PROCESSES] = sysAddProcess(my_process_inc, 4, argvInc,false,NULL);
  }

  for(i = 0; i < PAIR_PROCESSES; i++){
    sysWait(pids[i]);
    sysWait(pids[i + PAIR_PROCESSES]);
  }

  printk("Final value: %d\n", global);
}



// int64_t global;
// int64_t iterations = 0;
// void slowInc(int64_t *p, int64_t inc) {
//     int64_t aux = *p;
//     aux += inc;
//     sysYield();
//     *p = aux;
// }

// void inc(int argc, char *argv[]) {
//     uint64_t sem = satoi(argv[1]);
//     uint64_t value = satoi(argv[2]);
//     uint64_t copy = value;
//     uint64_t N = satoi(argv[3]);
//     uint64_t i;
//     if (sem && sysSemOpen(SEM_ID, 1) == -1) {
//         printErr("ERROR OPENING SEM\n");
//         return;
//     }
//     for (i = 0; i < N; i++) {
//         if (sem && sysSemWait(SEM_ID) != 0) {
//             printErr("ERROR WAITING SEM\n");
//         }
//         slowInc(&global, value);
//         if (sem && sysSemPost(SEM_ID) != 0) {
//             printErr("ERROR POSTING SEM\n");
//         }
//     }
//     if (sem && sysSemClose(SEM_ID) != 0) {
//         printErr("ERROR CLOSING SEM\n");
//     }
// }

// void semSyncTest(int argc, char ** argv) {
//     uint64_t i;
//     global = 0;
//     printk("CREATING PROCESSES...(WITH SEM)\n");
//     for ( int i = 0 ; i < PAIR_PROCESSES ; i++){
//         char *argv1[] = {"inc process with sem", "1", "1", "10000"};
//         sysAddProcess(&inc, 4, argv1, FOREGROUND, NULL);
//         char *argv2[] = {"inc process with sem", "1", "-1", "10000"};
//         sysAddProcess(&inc, 4, argv2, FOREGROUND, NULL);
//     }

// }

// void inc2(int argc, char *argv[]) {
//     uint64_t sem = satoi(argv[1]);
//     uint64_t value = satoi(argv[2]);
//     uint64_t N = satoi(argv[3]);
//     uint64_t i;
//     for (i = 0; i < N; i++) {
//         slowInc(&global, value);
//     }
//     printk("\nFinal value: %d\n", global);
// }

// void semNoSyncTest(int argc, char ** argv) {
//     uint64_t i;
//     global = 0;
//     printk("CREATING PROCESSES...(WITHOUT SEM)\n");
//     for (i = 0; i < PAIR_PROCESSES; i++) {
//         char *argv1[] = {"inc process without sem", "0", "1", "1000"};
//         sysAddProcess(&inc2, 4, argv1, FOREGROUND, NULL);
//         char *argv2[] = {"inc process without sem", "0", "-1", "1000"};
//         sysAddProcess(&inc2, 4, argv2, FOREGROUND, NULL);
//     }
// }



#ifndef SAMPLECODEMODULE_SYSCALLS_H
#define SAMPLECODEMODULE_SYSCALLS_H

#include "clock.h"
#include <stdint.h>

int sysWrite(uint64_t fd,const char *c, uint64_t length);
int sysRead(uint64_t fd,const char *c, uint64_t length);
void sysTime(clock * cptr);
int sysInfoReg(uint64_t * regs);
int sysPrintMem(uint8_t * mem, uint64_t address);
void sysSleep(uint64_t secs);
void sysClearScreen();
void sysTask(void * str, uint8_t flag);
void sysRunTasks();

#endif //SAMPLECODEMODULE_SYSCALLS_H

#ifndef SAMPLECODEMODULE_SYSCALLS_H
#define SAMPLECODEMODULE_SYSCALLS_H

#include "clock.h"
#include <stdint.h>
#include "commands.h"
typedef enum {READY, BLOCKED, TERMINATED} pState;

int sysWrite(uint64_t fd,const char *c, uint64_t length);
int sysRead(uint64_t fd,const char *c, uint64_t length);
void sysTime(clock * cptr);
int sysInfoReg(uint64_t * regs);
int sysPrintMem(uint8_t * mem, uint64_t address);
void sysSleep(uint64_t secs);
void sysClearScreen();

void * sysMalloc(uint64_t nBytes);
void sysFree(void * block);
int sysAddProcess(void (*entryPoint)(int, char**), int argc, char** argv, int foreground);
void sysWait(int pid);
int sysKillProcess(uint64_t pid);
int sysGetPID();
int sysBlockProcess(uint64_t pid);
int sysSetState(uint64_t pid, pState newState);
void sysSetPriority(uint64_t pid, int newPriority);
int sysReadyProcess(uint64_t pid);
int sysSemWait(uint32_t id);
int sysSemClose(uint32_t id);
int sysSemOpen(uint32_t id, uint64_t initialValue);
int sysSemPost(uint32_t id);
void sysSemStatus();
int sysPipeOpen(int pipeId);
int sysPipeClose(int pipeId);
int sysPipeRead(int pipeId);
int sysPipeWrite( int pipeId, char * src);
void sysYield();
int sysGetProcessList(processStruct * processStruct);



#endif //SAMPLECODEMODULE_SYSCALLS_H

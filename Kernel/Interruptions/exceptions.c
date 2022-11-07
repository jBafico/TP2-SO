// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>
#include <stdint.h>
#include <lib.h>
#include <keyboard.h>
#include <interrupts.h>
#include <int80Dispatcher.h>
#include <scheduler.h>

#define CANT_REGISTERS 17

typedef void (*Exception)(void);

static void zeroDivision();
static void invalidOpcode();

char* regNameArr[]={"RAX: ",
                    "RBX: ",
                    "RCX: ",
                    "RDX: ",
                    "RBP: ",
                    "RSI: ",
                    "RDI: ",
                    "RSP: ",
                    "R8:  ",
                    "R9:  ",
                    "R10: ",
                    "R11: ",
                    "R12: ",
                    "R13: ",
                    "R14: ",
                    "R15: ",
                    "RIP: "};

static Exception exceptions[]={&zeroDivision, 0, 0, 0, 0, 0, &invalidOpcode};
static uint64_t * registersAtException = NULL;

void exceptionHandler(char * errMsg){
    uint8_t fd = STDERR;

    ncClearFd(fd);
    ncPrintFdAttribute(fd, "OOPSIE WOOPSIE! A wild exception has appeared", Red, Black);
    ncNewLineFd(fd);
    ncPrintFdAttribute(fd, errMsg, Red, Black);
    ncNewLineFd(fd);
    for (int i = 0; i < CANT_REGISTERS; ++i) {
        ncPrintFdAttribute(fd, regNameArr[i], Red, Black);
        ncPrintHexFdAttribute(fd, registersAtException[i], Red, Black);
        ncNewLineFd(fd);
    }
    ncPrintFdAttribute(fd, "Press CTRL to return to SHELL", Red, Black);
    ncNewLineFd(fd);

    _sti();
    do{
        _hlt();
    }while(!getCtrlFlag());

    ncClearFd(fd);
    killCurrentFGProcess();
}

void exceptionDispatcher(int exception, uint64_t * registers) {
    Exception exFunc = exceptions[exception];
    registersAtException = registers;
	if(exFunc != 0){
		exFunc();
	}
}

static void zeroDivision() {
    char * errMsg= "Error: Program tried to divide by zero. Stopping process.";
    exceptionHandler(errMsg);
}

static void invalidOpcode() {
    char * errMsg= "Error: Invalid operation code. Stopping process.";
    exceptionHandler(errMsg);
}
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <lib.h>
#include <time.h>
#include <scheduler.h>
#include <interrupts.h>

#define CANTBYTES 32
#define MAX_BUFF 512

#define TRUE 1
#define FALSE !TRUE

enum sysCalls{  SYS_READ = 0,
                SYS_WRITE = 1,
                SYS_CLEARSCREEN = 69,
                SYS_MEM = 70,
                SYS_REGISTERS = 71,
                SYS_SLEEP = 72,
                SYS_TASK = 73,
                SYS_RUNTASKS = 74,
                SYS_TIME = 201};

#define NO_ARG_TASK 1
typedef void (*noArgPointer)(uint8_t fd);
typedef struct{
    noArgPointer function;
    uint8_t isLoop;
}noArgTask;

#define ARG_TASK 2
typedef void (*argPointer)(char * arg, uint8_t fd);
typedef struct{
    argPointer function;
    char * arg1;
}argTask;

static uint16_t reader = 0;

// Codigo sacado de: https://stackoverflow.com/questions/28133020/how-to-convert-bcd-to-decimal
static unsigned int bcdToDec(unsigned char time){
    return (time >> 4) * 10 + (time & 0x0F);
}

int sys_read(uint8_t fd, char * buff, uint64_t length){
    int writer;
    int i;
    char * kbdbuffer = getBuffer(&writer);

    if (reader == writer || fd != STDIN)
        return -1;

    uint8_t addedNewLine = FALSE;
    for (i = 0; i < length && !addedNewLine; i++, reader = ( reader + 1) % MAX_BUFF){
        buff[i] = kbdbuffer[reader];
        if (kbdbuffer[reader] == '\n')
            addedNewLine = TRUE;
    }
    return i;
}

int sys_write(uint8_t fd, char * buff, uint64_t length){
    if(buff == NULL)
        return 0;

    uint8_t color = White;
    //seteo color rojo en caso de que sea STDERR
    if (fd == STDERR || fd == STDERRDER ||fd == STDERRIZQ ||  fd == STDERRBOTH)
        color = Red;

    int i;
    for (i = 0; i < length; ++i) {
        if ( buff[i] == '\n')
            ncNewLineFd(fd);
        else if ( buff[i] == '\b')
            ncDeleteChar();
        else if(buff[i]=='\t')
            ncPrintFdAttribute(fd, "   ", color, Black);
        else
            ncPrintCharFdAttribute(fd, buff[i], color, Black);
    }

    return i;
}

void sys_time(clock * str){
    if(str == NULL)
        return;

    str->seconds = bcdToDec(getSeconds());
    str->minutes = bcdToDec(getMins());
    uint8_t hrs=bcdToDec(getHour());
    if(hrs<3)
        hrs+=24;
    hrs-=3;
    str->hours = hrs;
    str->dayOfMonth = bcdToDec(getDayOfMonth());
    str->month = bcdToDec(getMonth());
    str->year = bcdToDec(getYear());
}

void sys_sleep(uint64_t secs){
    uint64_t current_secs = seconds_elapsed();
    uint64_t final_secs=current_secs+secs;
    while(current_secs<final_secs){
        current_secs=seconds_elapsed();
    }
}

void sys_clearscreen(){
    ncClear();
}

//int sys_registers(uint64_t regs[]){
//    return getRegisters(regs);
//}

int sys_mem(uint8_t * mem, uint64_t address){
    if((uint64_t) address > (0x20000000 - 32)) {
        return -1;
    }
    // cargo en el array mem 32 bytes a partir de address
    for (int i = 0; i < CANTBYTES; i++) {
        mem[i] = getByte(address);
        address++;
    }
    return 0;
}

//void sys_task(void * str, uint8_t flag){
//    addTask(str, flag);
//}

//void sys_run(){
//    runTasks();
//}

int _int80Dispatcher(uint16_t code, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    _sti();
    switch (code) {
        case SYS_READ: //arg0: fd , arg1: buff, arg2: length
            return sys_read( (uint8_t) arg0, (char *) arg1, (uint64_t) arg2);
        case SYS_WRITE: //arg0: fd , arg1: buff, arg2: length
            return sys_write( (uint8_t) arg0, (char *) arg1, (uint64_t) arg2);
        case SYS_TIME: //arg0: clock * donde va a guardar la info
            sys_time((clock *) arg0);
            break;
        case SYS_CLEARSCREEN:
            sys_clearscreen();
            break;
        case SYS_SLEEP:
            sys_sleep((uint64_t) arg0);
            break;
        case SYS_MEM:  //arg0: uint8_t * mem, array de 32 lugares de 8bits, arg1: uint64_t address, direc para buscar
            return sys_mem((uint8_t *) arg0, (uint64_t) arg1);
//        case SYS_TASK:
//            sys_task((void *) arg0, (uint8_t) arg1);
            break;
//        case SYS_RUNTASKS:
//            sys_run();
            break;
//        case SYS_REGISTERS:
//            return sys_registers( (uint64_t *) arg0);
    }
    return 0;
}

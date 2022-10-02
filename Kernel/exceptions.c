#include "naiveConsole.h"
#include "stdint.h"
#include "lib.h"
#include "interrupts.h"
#include "keyboard.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE 6

typedef void (*Exception)(void);

static void * const sampleCodeModuleAddress = (void*)0x400000;

typedef int (*EntryPoint)();

static void zeroDivision();
static void invalidOpcode();

char* regNameArr[]={"rax: ", "rbx: ", "rcx: ", "rdx: ", "rbp: ", "rsi: ", "rdi: ", "rsp: ", "r8: ","r9: ","r10: ","r11: ","r12: ","r13: ","r14: ","r15: "};

static Exception exceptions[]={&zeroDivision, 0, 0, 0, 0, 0, &invalidOpcode};

static void zeroDivision() {
    char * errMsg= "Error: Program tried to devide by zero. Stopping process.";
    exceptionHandler(errMsg);
}

static void invalidOpcode() {
    char * errMsg= "Error: Invalid operation code. Stopping process.";
    exceptionHandler(errMsg);
}

void exceptionHandler(char * errMsg){
    ncPrintAttribute(errMsg, Red, Black); //TODO Revisar como queremos hacer que imprima la salida de error
    ncNewline();
    //TODO poner la funcion que imprime los estados de registros
    uint64_t * regArr= prepareRegisters();
    for (int i = 0; i < 16; ++i) {
        ncPrint(regNameArr[i]);
        ncPrintHex(regArr[i]);
        ncNewline();
    }
    ncNewline();
    //Seccion de frenado de Procesos
    ncPrintAttribute("Ingrese ENTER para poder continuar", Red, Black);
    int c;
    
    do{
        _hlt();//hlt frena el CPU hasta que se detecte la proxima interrupcion externa
    }while((c=getCharKernel()) != '\n');
    ncClear();
    ((EntryPoint)sampleCodeModuleAddress)();//TODO revisar esto
    //aca hay que volver a darle el control al usuario
    
}

void exceptionDispatcher(int exception) {
    Exception exFunc = exceptions[exception];
	if(exFunc != 0){
		exFunc();
	}
}
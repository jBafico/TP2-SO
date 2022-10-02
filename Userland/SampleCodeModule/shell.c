#include "shell.h"
#include "library.h"
#include "clock.h"
#include "help.h"
#include "fibonacci.h"
#include "primeNums.h"
#include "exceptionTester.h"

#define MAX_LEN_COMMAND 128
#define NUM_COMMANDS 8

//defino una memoria auxiliar para mi vector de strings
#define AUXVECTORLENGTH 20

typedef void (*commandPointer)(void);

typedef struct{
    char * name;
    commandPointer function;
}command;

static command availableCommands[NUM_COMMANDS] = {{"help", &help},
                                   { "divZero", &divideZero},
                                   { "invalidOpCode", &invalidOpCode},
                                   {"inforeg", &infoReg},
                                   {"printmem", &printMem},
                                   {"time", &time},
                                   {"primos", &primeNumbers},
                                   {"fibonacci", &fibo},
                                   };



//funcion en probacion
static int parseString(char *target, char **vec){
    int aux = 0;
    int j = 0;
    int spaceBefore = FALSE;
    for ( int i = 0; target[i] != 0 ; i++){
        if ( target[i] != ' '){
            spaceBefore = FALSE;
            vec[aux][j++] = target[i];
        } else {
            vec[aux][j] = 0;
            if ( !spaceBefore ){
                aux++;
                j = 0;
                spaceBefore = TRUE;
            }
        }
    }
    return aux;
}



void stopForCommand(){

    char c;
    char currentLine[MAX_LEN_COMMAND];
    int i = 0;

    while((c=getChar()) != '\n'){
        if(i>0 && c == '\b'){
            putCharacter(STDOUT, c);
            currentLine[--i] = 0;
        }
        else if(c != '\b') {
            putCharacter(STDOUT, c);
            currentLine[i++] = c;
        }
    }

    printk("\n");

    currentLine[i] = 0;

    //probaciones
    char strings[AUXVECTORLENGTH][MAX_LEN_COMMAND];
    int stringsDim = parseString(currentLine , strings);
    if(stringsDim==1){
        //execute para funciones que no reciban args
    }else if(stringsDim==2){
        //execute para funciones con un arg
    }else{

    }

    
    if(!execute(currentLine)){
        printErr(currentLine);
        printErr(" : comando no encontrado\n");
    }
    printk("\n");
}

int execute(char * command){

    for(int i=0 ; i < NUM_COMMANDS; ++i){
        if(strcmp(command,availableCommands[i].name) == 0){
            availableCommands[i].function();
            return TRUE;
        }
    }

    return FALSE;
}
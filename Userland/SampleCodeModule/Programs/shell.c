/*
#include <shell.h>
#include <library.h>
#include <clock.h>
#include <help.h>
#include <fibonacci.h>
#include <primeNums.h>
#include <exceptionTester.h>
#include <printMem.h>
#include <sysCalls.h>


#define MAX_LEN_COMMAND 128
#define AUXVECTORLENGTH 20

#define PIPE "|"

typedef struct{
    char * name;
    noArgPointer function;
}noArgCommand;

typedef struct{
    char * name;
    argPointer function;
}argCommand;

#define NUM_COMMANDS_NO_ARG 5
static noArgCommand noArgumentCommands[NUM_COMMANDS_NO_ARG] = {{"help", &help},
                                   { "divZero", &divideZero},
                                   { "invalidOpCode", &invalidOpCode},
//                                   {"inforeg", &infoReg},
                                   {"time", &time},
                                   };

#define NUM_LOOP_COMMANDS 2
static noArgCommand loopCommands[NUM_LOOP_COMMANDS]= {{"primos", &nextPrime},
                                                      {"fibonacci", &nextFibo}};

#define NUM_COMMANDS_ARG 1
static argCommand argumentCommands[NUM_COMMANDS_ARG] = {{"printmem", &printMem}};



enum inputCases{SINGLE_FUNCTION = 1, SINGLE_FUNC_W_ARG, PIPE_NO_ARGS, PIPE_ONE_ARG, PIPE_TWO_ARGS};

#define CANT_ERR_MESSAGES 3
char * errMessages[CANT_ERR_MESSAGES] = {" : comando no encontrado\n",
                                        "Combinacion de argumentos no valida\n",
                                        " : comando no encontrado / Combinacion invalida de argumentos\n"};



#define IS_SPACE_OR_TAB(c) ((c) == ' ' || (c) == '\t')
// funcion para parsear el string, cada fila de la matriz es un string
// devuelve cantidad de palabras (incluyendo el pipe) que encontro
int parseString(char m[][MAX_LEN_COMMAND], const char * src){
    int dim = 0;
    int j = 0;
    int i = 0;

    //salteo los primeros  espacios
    while (IS_SPACE_OR_TAB(src[i]))
        i++;

    while (src[i] != 0){
        if (!IS_SPACE_OR_TAB(src[i])){
            m[dim][j++] = src[i];
            //si estoy terminando el string!
            if ( src[i + 1] == 0){
                m[dim][j] = 0;
                dim++;
            }
            i++;
        } else {
            //completo String anterior
            m[dim][j] = 0;
            while (IS_SPACE_OR_TAB(src[i]))
                i++;
            //aumento dimension de la matriz
            dim++;
            j = 0;
        }
    }
    return dim;
}

void errArguments(char * str, char * errMsg, char * errFlag){
    printkfd(STDERR, str);
    printkfd(STDERR, errMsg);
    *errFlag=TRUE;
}

void stopForCommand(){
    char c;
    char currentLine[MAX_LEN_COMMAND];
    int i = 0;
    restartStates();

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

    char strings[AUXVECTORLENGTH][MAX_LEN_COMMAND];
    int stringsDim = parseString(strings , currentLine);
    char errFlag = FALSE;
    char pipeFlag=FALSE;
    argTask argTask1, argTask2;
    noArgTask noArgTask1, noArgTask2;

    switch (stringsDim) {
        case SINGLE_FUNCTION:
            if(!addLoopFunc(&noArgTask1,strings[0]) && !addNoArgFunc(&noArgTask1,strings[0]))
                errArguments(strings[0], errMessages[0], &errFlag);
            break;
        case SINGLE_FUNC_W_ARG:
            if(!addArgFunc(&argTask1,strings[0],strings[1])){
                errArguments(strings[0], errMessages[2], &errFlag);
            }
            break;
        case PIPE_NO_ARGS:
            if(strcmp(strings[1],PIPE) == 0){
                pipeFlag=TRUE;
                if(!addLoopFunc(&noArgTask1,strings[0]) && !addNoArgFunc(&noArgTask1,strings[0]))
                    errArguments(strings[0], errMessages[0], &errFlag);
                if(!addLoopFunc(&noArgTask2,strings[2]) && !addNoArgFunc(&noArgTask2,strings[2]))
                    errArguments(strings[2], errMessages[0], &errFlag);
            }
            else{
                printErr(errMessages[1]);
                errFlag=TRUE;
            }
            break;
        case PIPE_ONE_ARG:
            if(strcmp(strings[1],PIPE) == 0){
                pipeFlag=TRUE;
                if(!addLoopFunc(&noArgTask1,strings[0]) && !addNoArgFunc(&noArgTask1,strings[0]))
                    errArguments(strings[0], errMessages[0], &errFlag);
                if(!addArgFunc(&argTask2,strings[2],strings[3]))
                    errArguments(strings[2], errMessages[2], &errFlag);
            }
            else if(strcmp(strings[2],PIPE) == 0){
                pipeFlag=TRUE;
                if(!addArgFunc(&argTask1,strings[0],strings[1]))
                    errArguments(strings[0], errMessages[2], &errFlag);
                if(!addLoopFunc(&noArgTask2,strings[3]) && !addNoArgFunc(&noArgTask2,strings[3]))
                    errArguments(strings[3], errMessages[0], &errFlag);
            }
            else{
                printErr(errMessages[1]);
                errFlag=TRUE;
            }
            break;
        case PIPE_TWO_ARGS:
            if(strcmp(strings[2],PIPE) == 0){
                pipeFlag=TRUE;
                if(!addArgFunc(&argTask1,strings[0],strings[1]))
                    errArguments(strings[0], errMessages[2], &errFlag);
                if(!addArgFunc(&argTask2,strings[3],strings[4]))
                    errArguments(strings[3], errMessages[2], &errFlag);
            }
            else{
                printErr(errMessages[1]);
                errFlag=TRUE;
            }
            break;
        default:
            printErr(errMessages[1]);
            errFlag=TRUE;
    }

    if(!errFlag){
        if(pipeFlag)
            sysClearScreen();
        sysRunTasks();
    }
}

void restartStates(){
    restartFibo();
    restartPrimes();
}

int addLoopFunc(noArgTask * str, char * command){
    for(int i=0 ; i < NUM_LOOP_COMMANDS; ++i){
        if(strcmp(command,loopCommands[i].name) == 0){
            str->isLoop=TRUE;
            str->function=loopCommands[i].function;
            sysTask(str, NO_ARG_TASK);
            return TRUE;
        }
    }
    return FALSE;
}

int addNoArgFunc(noArgTask * str, char * command){
    for(int i=0 ; i < NUM_COMMANDS_NO_ARG; ++i){
        if(strcmp(command,noArgumentCommands[i].name) == 0){
            str->isLoop=FALSE;
            str->function=noArgumentCommands[i].function;
            sysTask(str, NO_ARG_TASK);
            return TRUE;
        }
    }
    return FALSE;
}

int addArgFunc(argTask * str, char * command, char * arg){
    for(int i = 0 ; i < NUM_COMMANDS_ARG; ++i){
        if(strcmp(command,argumentCommands[i].name) == 0){
            str->function=argumentCommands[i].function;
            str->arg1=arg;
            str->isLoop=FALSE;
            sysTask(str, ARG_TASK);
            return TRUE;
        }
    }
    return FALSE;
}

*/
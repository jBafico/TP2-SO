#include <shell.h>
#include <library.h>
#include <clock.h>
#include <help.h>
#include <exceptionTester.h>
#include <printMem.h>
#include <sysCalls.h>
#include <semTest.h>
#include <processTest.h>
#include <mmTest.h>
#include <prioTest.h>
#include <phylo.h>

#define MAX_LEN_COMMAND 128
#define AUXVECTORLENGTH 20

#define PIPE "|"
#define AMPER "&"

typedef struct command{
    char * name;
    void (* function) (int, char **);
}command;

#define CANT_NO_PIPE_COMMS 23
command noPipeComms[CANT_NO_PIPE_COMMS]= {{"help",     &help},
                                        {"time",          &time},
                                        {"invalidOpCode", &invalidOpCode},
                                        {"divZero",       &divideZero},
//                                        {"mem",           &mem},
                                        {"ps",            &ps},
//                                        {"sem",           &sem},
//                                        {"pipe",          &pipe},
//                                        {"phylo",         &phylo},
                                        {"memTest",       &memTest},
                                        {"prioTest",      &prioTest},
                                        {"processTest",   &processTest},
                                        {"semSyncTest",   &semSyncTest},
                                        {"semNoSyncTest", &semNoSyncTest},
//                                        {"kill",          &kill},
//                                        {"nice",          &nice},
//                                        {"block",         &block}
                                        };

//
#define CANT_PIPE_COMMS 5
command pipeComms[CANT_PIPE_COMMS] = {
//                                        {"cat", &cat},
//                                      {"wc", &wc},
//                                      {"loop", &loop},
//                                      {"filter", &filter},
                                      {"phylo", &phyloProblem}};



#define CANT_ERR_MESSAGES 4
char * errMessages[CANT_ERR_MESSAGES] = {" : comando no encontrado\n",
                                        "Combinacion de argumentos no valida\n",
                                        " : comando no encontrado / Combinacion invalida de argumentos\n",
                                        "El pipe ('|') debe usarse unicamente entre dos funciones (ejecutar /help para ver las permitidas)"};



#define IS_SPACE_OR_TAB(c) ((c) == ' ' || (c) == '\t')
// funcion para parsear el string, cada fila de la matriz es un string
// devuelve cantidad de palabras (incluyendo el pipe) que encontro
//  "pepe roro dfasdfasdfadsfasdfa"
static int parseString(char m[][MAX_LEN_COMMAND], const char * src){
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
            if (src[i + 1] == 0){
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

static void errArguments(char * str, char * errMsg, char * errFlag){
    printkfd(STDERR, str);
    printkfd(STDERR, errMsg);
    *errFlag=true;
}

int addNoPipeFunc(char ** argv, int argc, bool backgroundFlag){
    for(int i = 0 ; i < CANT_NO_PIPE_COMMS; ++i){
        if(strcmp(argv[0],noPipeComms[i].name) == 0){
            if(sysAddProcess(noPipeComms[i].function, argc, argv, !backgroundFlag) == ERROR)
                return ERROR;
            return 0;
        }
    }
    return ERROR;
}

//TODO: conectar con pipes a los procesos
//Pipe processes will always run in foreground
int addPipeFunc(char ** argv, int argc){
    bool firstAdded = false, secondAdded = false;

    for(int i = 0 ; i < CANT_PIPE_COMMS; ++i){
        if(strcmp(argv[0],pipeComms[i].name) == 0){
            if(sysAddProcess(pipeComms[i].function, argc, argv, true) == ERROR)
                return ERROR;
            firstAdded = true;
        }
        else if(strcmp(argv[2],pipeComms[i].name) == 0){
            if(sysAddProcess(pipeComms[i].function, argc, argv, true) == ERROR)
                return ERROR;
            secondAdded = true;
        }

        if(firstAdded && secondAdded)
            return 0;
    }
    return ERROR;
}

void prepareArgv(char * dest[], char src[][MAX_LEN_COMMAND], int argc){
    for (int i = 0; i < argc; ++i) {
        dest[i] = src[i];
    }
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
    int argc = parseString(strings , currentLine);
    bool errFlag = false;
    bool pipeFlag = false;
    bool backgroundFlag = false;


    /*
     * TODO:
     * Casos posible:
     * 1) Funcion sola (con o sin args)
     * 2) Pipe -> siempre va a estar en posicion 1 del array
     */

    if(strcmp(strings[1],PIPE) == 0){
        //Pipe functions do not recieve arguments, so format will be "func1 | func2"
        if(argc == 3)
            pipeFlag = true;
        else{
            printErr(errMessages[3]);
            errFlag=true;
        }
    }
    else if(argc > 1 && strcmp(strings[--argc],AMPER) == 0)
        backgroundFlag = true;


    if(!errFlag){
        char * argv[AUXVECTORLENGTH];
        prepareArgv(argv, strings, argc);
        sysClearScreen();

        if(pipeFlag)
            addPipeFunc(argv, argc);
        else
            addNoPipeFunc(argv, argc, backgroundFlag);

    }
}










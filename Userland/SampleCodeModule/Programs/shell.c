#include <shell.h>
#include <library.h>
#include <clock.h>
#include <help.h>
#include <exceptionTester.h>
#include <sysCalls.h>
#include <semTest.h>
#include <processTest.h>
#include <mmTest.h>
#include <prioTest.h>
#include <phylo.h>

typedef struct command{
    char * name;
    void (* function) (int, char **);
}command;

#define CANT_NO_PIPE_COMMS 23
command noPipeComms[CANT_NO_PIPE_COMMS]= {{"help",     &help},
                                        {"time",          &time},
                                        {"mem",           &mem},
                                        {"ps",            &ps},
//                                        {"sem",           &sem},
//                                        {"pipe",          &pipe},
                                        {"memTest",       &memTest},
                                        {"prioTest",      &prioTest},
                                        {"processTest",   &processTest},
                                        {"semSyncTest",   &semSyncTest},
                                        {"semNoSyncTest", &semNoSyncTest},
                                        {"kill",          &kill},
                                        {"nice",          &nice},
                                        {"block",         &block},
                                        {"unblock",       &unblock}
                                        };

//
#define CANT_PIPE_COMMS 5
command pipeComms[CANT_PIPE_COMMS] = {
//                                        {"cat", &cat},
//                                      {"wc", &wc},
                                      {"loop", &loop},
//                                      {"filter", &filter},
                               {"phylo", &phyloProblem}
};



#define CANT_ERR_MESSAGES 6
char * errMessages[CANT_ERR_MESSAGES] = {" : comando no encontrado\n",
                                        "Combinacion de argumentos no valida\n",
                                        " : comando no encontrado / Combinacion invalida de argumentos\n",
                                        "El pipe ('|') debe usarse unicamente entre dos funciones (ejecutar /help para ver las permitidas)\n",
                                        "El proceso no pudo ser creado correctamente\n",
                                        "El pipe no pudo ser abierto correctamente\n"};



#define IS_SPACE_OR_TAB(c) ((c) == ' ' || (c) == '\t')

static int pipeId = 100;


void initalizeShell(int argc, char ** argv){
    printk("OMG USER HI!! Welcome to the LettuceOS Shell!\n\n");
    help(0,NULL);



    while (1){
        printk("> ");
        stopForCommand();
    }
}

// funcion para parsear el string, cada fila de la matriz es un string
// devuelve cantidad de palabras (incluyendo el pipe) que encontro
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

static void errArguments(char * str, char * errMsg){
    printkfd(STDERR, str);
    printkfd(STDERR, errMsg);
}

int addNoPipeFunc(char ** argv, int argc, bool backgroundFlag){
    for(int i = 0 ; i < CANT_NO_PIPE_COMMS; ++i){
        if(strcmp(argv[0],noPipeComms[i].name) == 0){
            if(sysAddProcess(noPipeComms[i].function, argc, argv, !backgroundFlag, NULL) == ERROR)
                return ADD_PROC_ERROR;
            return 0;
        }
    }
    for(int i = 0 ; i < CANT_PIPE_COMMS; ++i){
        if(strcmp(argv[0],pipeComms[i].name) == 0){
            if(sysAddProcess(pipeComms[i].function, argc, argv, !backgroundFlag, NULL) == ERROR)
                return ADD_PROC_ERROR;
            return 0;
        }
    }
    return ERROR;
}

void getPipeFunc(char * n1, char * n2, command * f1, command * f2){
    for (int i = 0; i < CANT_PIPE_COMMS; i++) {
        if(strcmp(n1, pipeComms[i].name) == 0)
            *f1 = pipeComms[i];
        else if(strcmp(n2 ,pipeComms[i].name) == 0)
            *f2 = pipeComms[i];
    }
}

//Pipe processes will always run in foreground
int addPipeFunc(char ** argv){
    char * argv1[1] = {argv[0]};
    char * argv2[1] = {argv[2]};

    command f1 = {NULL, NULL};
    command f2 = {NULL, NULL};

    getPipeFunc(argv[0], argv[2], &f1, &f2);

    if(f1.name == NULL && f2.name == NULL)
        return ERROR;
    else if (f1.name == NULL)
        return FIRST_ERROR;
    else if(f2.name == NULL)
        return SECOND_ERROR;

    int pipe = sysPipeOpen(pipeId++);

    if(pipe == ERROR)
        return PIPE_ERROR;

    int fd1[2] = {pipe, 1};
    int fd2[2] = {0, pipe};

    if(sysAddProcess(f1.function, 1, argv1, true, fd1) == ERROR){
        sysPipeClose(pipe);
        return ADD_PROC_ERROR;
    }

    if(sysAddProcess(f2.function, 1, argv2, true, fd2) == ERROR){
        sysPipeClose(pipe);
        return ADD_PROC_ERROR;
    }

    int endOfFile = EOF;
    sysPipeWrite(pipe, (char *)&endOfFile);

    sysPipeClose(pipe);
    printk("\n");

    return 0;
}

void handlePipe(char ** argv, int argc){
    if(argc != 3) {
        printErr(errMessages[3]);
    }
    else {
        switch (addPipeFunc(argv)) {
            case FIRST_ERROR:
                errArguments(argv[0], errMessages[0]);
                break;
            case SECOND_ERROR:
                errArguments(argv[2], errMessages[0]);
                break;
            case ERROR:
                errArguments(argv[0], errMessages[0]);
                errArguments(argv[2], errMessages[0]);
                break;
            case ADD_PROC_ERROR:
                printErr(errMessages[4]);
                break;
            case PIPE_ERROR:
                printErr(errMessages[5]);
                break;
        }
    }
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

    char strings[AUXVECTORLENGTH][MAX_LEN_COMMAND] = {0};
    int argc = parseString(strings , currentLine);
    bool backgroundFlag = false;

    /*
     * Casos posible:
     * 1) Funcion sola (con o sin args)
     * 2) Pipe -> siempre va a estar en posicion 1 del array
     */

    if(argc > 0){
        char * argv[AUXVECTORLENGTH] = {0};
        prepareArgv(argv, strings, argc);

        //Pipe functions do not recieve arguments, so format will be "func1 | func2"
        if(strcmp(strings[1],PIPE) == 0)
            handlePipe(argv, argc);
        else{
            if(argc > 1 && strcmp(strings[--argc],AMPER) == 0)
                backgroundFlag = true;

            switch (addNoPipeFunc(argv, argc, backgroundFlag)) {
                case ERROR:
                    errArguments(argv[0],errMessages[0]);
                    break;
                case ADD_PROC_ERROR:
                    printErr(errMessages[4]);
                    break;
            }
        }
    }
}










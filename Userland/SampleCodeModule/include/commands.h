//
// Created by Franco Bosetti on 31/10/2022.
//

#ifndef SAMPLECODEMODULE_COMMANDS_H
#define SAMPLECODEMODULE_COMMANDS_H


//nombre, ID,
//prioridad, stack y base pointer, foreground
#define EXCHANGEBUFFERSIZE 128
#define SMALLBUFFER 64

#include <stdint.h>
typedef struct processStruct{
    uint64_t pid;
    char name[EXCHANGEBUFFERSIZE];
    uint64_t stackPointer;
    uint64_t basePointer;
    char type[SMALLBUFFER];
    char state[SMALLBUFFER];
} processStruct;

void ps(int argc, char ** argv);

#endif //SAMPLECODEMODULE_COMMANDS_H

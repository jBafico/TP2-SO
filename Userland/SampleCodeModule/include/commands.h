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

typedef struct processInfo{
    uint64_t pid;
    char name[EXCHANGEBUFFERSIZE];
    uint64_t stackPointer;
    uint64_t basePointer;
    char type[SMALLBUFFER];
    char state[SMALLBUFFER];
} processInfo;

void ps(int argc, char ** argv);
void unblock(int arg, char ** argv);
void block(int argc, char ** argv);
void nice(int argc, char ** argv);
void kill(int arg, char ** argv);
void loop(int argc, char ** argv);
void mem(int argc, char ** argv);
void semInfo(int arg, char ** argv);
void cat(int arg, char ** argv);
void wc(int arg, char ** argv);
void filter(int arg, char ** argv);

#endif //SAMPLECODEMODULE_COMMANDS_H

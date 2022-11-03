#ifndef SAMPLECODEMODULE_SHELL_H
#define SAMPLECODEMODULE_SHELL_H

#include <stdint.h>

#define EOF 96

#define MAX_LEN_COMMAND 128
#define AUXVECTORLENGTH 20

#define PIPE "|"
#define AMPER "&"

#define FIRST_ERROR (-3)
#define SECOND_ERROR (-4)
#define PIPE_ERROR (-5)

void stopForCommand();
void initalizeShell(int argc, char ** argv);

#endif //SAMPLECODEMODULE_SHELL_H


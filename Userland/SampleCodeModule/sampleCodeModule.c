// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <library.h>
#include <shell.h>
#include <sysCalls.h>

int main() {
    char * argv[] = {"Shell"};
    sysAddProcess(initalizeShell,1, argv,true, NULL);
    return 0;
}

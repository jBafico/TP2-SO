#include <library.h>
#include <shell.h>
#include <sysCalls.h>

int main() {
    char * argv[] = {"Shell"};
    sysAddProcess(initalizeShell,1, argv,true, NULL);
    return 0;
}

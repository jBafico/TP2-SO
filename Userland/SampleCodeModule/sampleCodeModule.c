#include <library.h>
#include <shell.h>
#include <help.h>
#include <sysCalls.h>

int main() {
    char * argv[] = {"Shell"};
    sysAddProcess(initalizeShell,1,argv,1);
    return 0;
}

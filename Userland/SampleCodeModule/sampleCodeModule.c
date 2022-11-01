#include <library.h>
#include <shell.h>
#include <help.h>
#include <sysCalls.h>

#define TRUE 1
#define FALSE !TRUE

static int firstEntry = TRUE;

int main() {
    
    char * argv[] = {"Shell"};
    sysAddProcess(initalizeShell,1,argv,1);
    return 0;
}

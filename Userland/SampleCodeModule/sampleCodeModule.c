#include "library.h"
#include "shell.h"
#define TRUE 1
#define FALSE !TRUE

static int firstEntry = TRUE;

int main() {
    //esta va a ser mi shell
    //consumo hasta el newline

    printk("ranita OS shell\n");

    while ( firstEntry && getChar() != '\n')
        ;

    firstEntry = FALSE;

    while (1){
        printk("> ");
        stopForCommand();
    }

    return 0;
}
#include <library.h>
#include <sysCalls.h>

static int pipeId= 300;
static void writer(int argc, char ** argv){
    int pid = sysGetPID();
    printk("Hey! I'm process %d. They will delete my vocals :(\n", pid);
}
static void reader(int argc, char ** argv){
    char c;
    while ((c = getChar()) != EOF) {
        if (!isVowel(c))
            putCharacter(STDOUT, c);
    }
    putCharacter(STDOUT, '\n');
}

void pipeTest(int argc, char ** argv){ //{n, use_sem}
   
    printk("Starting to create processes and pipe\n");
    char *argv1[] = {"Test Writer"};
    char *argv2[] = {"Test Reader"};
    

    int pipe = sysPipeOpen(pipeId++);

    if(pipe == ERROR)
        return ;

    int fd1[2] = {STDIN, pipe};

    if(sysAddProcess(&writer, 1, argv1, true, fd1) == ERROR){
        sysPipeClose(pipe);
        return ;
    }
    printk("First process conected\n");
    sysSleep(5);
    char endOfFile = EOF;
    sysPipeWrite(pipe, &endOfFile);

    int fd2[2] = {pipe, STDOUT};

    if(sysAddProcess(&reader, 1, argv2, true, fd2) == ERROR){
        sysPipeClose(pipe);
        return ;
    }
    printk("Second process conected\n");
    sysSleep(5);
    sysPipeWrite(pipe, &endOfFile);
    sysPipeClose(pipe);
    printk("Test finished\n");
    return 0;
}

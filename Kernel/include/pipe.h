#ifndef KERNEL_PIPE_H
#define KERNEL_PIPE_H
#define SMALLBUFFER 64
typedef struct pipeData {
    int id; // id of the pipe
    char readState[SMALLBUFFER];    // read fd is blocked
    char writeState[SMALLBUFFER];  // write fd is blocked
    int processesUsing;
} pipeData;

int pipeOpen(int pipeId);
int pipeClose(int pipeId);
int pipeWrite(int pipeId, const char * src);
int pipeRead(int pipeId);
int pipeInfo(pipeData * s);

#endif //KERNEL_PIPE_H

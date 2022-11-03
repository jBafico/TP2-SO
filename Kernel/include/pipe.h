#ifndef KERNEL_PIPE_H
#define KERNEL_PIPE_H

int pipeOpen(int pipeId);
int pipeClose(int pipeId);
int pipeWrite(int pipeId, char * src);
int pipeRead(int pipeId);

#endif //KERNEL_PIPE_H

#ifndef KERNEL_INT80DISPATCHER_H
#define KERNEL_INT80DISPATCHER_H
#include <stdint.h>
int sys_write(char * buff, uint64_t length);
int sys_read(char * buff, uint64_t length);

#endif //KERNEL_INT80DISPATCHER_H
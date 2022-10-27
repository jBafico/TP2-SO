#ifndef KERNEL_INT80DISPATCHER_H
#define KERNEL_INT80DISPATCHER_H
#include <stdint.h>
int sys_read(uint8_t fd, char * buff, uint64_t length);

#endif //KERNEL_INT80DISPATCHER_H
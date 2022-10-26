#ifndef TP2_SO_MEMMANAGER_H
#define TP2_SO_MEMMANAGER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void initMemMan(char *heapBase, size_t heapSize);
void *malloc(uint64_t nBytes);
void free(void *block);

#endif //TP2_SO_MEMMANAGER_H

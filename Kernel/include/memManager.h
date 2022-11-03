#ifndef TP2_SO_MEMMANAGER_H
#define TP2_SO_MEMMANAGER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct info {
    uint64_t availableBytes;
    uint64_t allocatedBytes;
    uint64_t totalMemory;
} memInfo;

void initMemMan(void *heapBase, size_t heapSize);
void * malloc(uint64_t nBytes);
void free(void *block);

#endif //TP2_SO_MEMMANAGER_H

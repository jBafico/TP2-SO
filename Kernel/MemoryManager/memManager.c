#include "../include/memManager.h"


//Implementation Based on K&R Book

typedef long Align;     //To align upper limit

typedef union header {  //Block Header
    struct {
        union header *ptr;  //Next Block if it is on the free list
        size_t size;        //Block Size
    } data;

    Align x;                //Obliga a la alineación de bloques

} Header;

static Header * base;     // empty list to initialize
static Header * freePtr=NULL;   // Start of the next free Node
size_t totalUnits;

void initMemMan(char *heapBase, size_t heapSize) {
    if (heapBase == NULL) {
        return;
    }
    totalUnits = (heapSize + sizeof(Header) - 1) / sizeof(Header) + 1;
    freePtr = base = (Header *)heapBase;
    freePtr->data.size = totalUnits;
    freePtr->data.ptr = freePtr;
}

void * malloc(uint64_t nBytes) {
    Header *currentNode, *prevPtr;
    size_t nUnits;
    void *ret;
    bool allocFlag; //Flag used to cut the for loop when allocation is finished

    nUnits = (nBytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    prevPtr = freePtr;

    allocFlag = true; //Going to start allocating
    for (currentNode = prevPtr->data.ptr; allocFlag; currentNode = currentNode->data.ptr) {
        if (currentNode->data.size >= nUnits) {
            if (currentNode->data.size == nUnits) {
                prevPtr->data.ptr = currentNode->data.ptr;
            } else {
                currentNode->data.size -= nUnits;
                currentNode += currentNode->data.size;
                currentNode->data.size = nUnits;
            }

            freePtr = prevPtr;
            ret = currentNode + 1;
            allocFlag = false;  //Finished allocating
        }
        if (currentNode == freePtr) {
            return NULL;
        }

        prevPtr = currentNode;
    }
    return ret;
}

void free(void *block) {
    if (block == NULL) {
        return;
    }

    Header *freeBlock, *currentNode;
    freeBlock = (Header *)block - 1;

    if (freeBlock < base ||
        freeBlock >= (base + totalUnits * sizeof(Header))) {
        return;
    }

    block = NULL;

    bool found = false; //flag to check if block to free is in mem

    for (currentNode = freePtr;
         !(freeBlock > currentNode && freeBlock < currentNode->data.ptr);
         currentNode = currentNode->data.ptr) {
        if (freeBlock == currentNode || freeBlock == currentNode->data.ptr) {
            return;
        }
        if (currentNode >= currentNode->data.ptr &&
            (freeBlock > currentNode || freeBlock < currentNode->data.ptr)) {
            found = true;   //block found
            break; // Frees start and end of the block
        }
    }

    if (!found &&
        (currentNode + currentNode->data.size > freeBlock ||
         freeBlock + freeBlock->data.size > currentNode->data.ptr)) {
        return;     //block does not exist
    }

    if (freeBlock + freeBlock->data.size == currentNode->data.ptr) { // connects next block
        freeBlock->data.size += currentNode->data.ptr->data.size;

        freeBlock->data.ptr = currentNode->data.ptr->data.ptr;

    } else {
        freeBlock->data.ptr = currentNode->data.ptr;
    }

    if (currentNode + currentNode->data.size == freeBlock) {    //connects previous block
        currentNode->data.size += freeBlock->data.size;

        currentNode->data.ptr = freeBlock->data.ptr;

    } else {
        currentNode->data.ptr = freeBlock;
    }

    freePtr = currentNode;
}

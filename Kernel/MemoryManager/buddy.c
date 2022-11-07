// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memManager.h>
#define STRUCT_POS 0x600000
#define MIN_BLOCK 64


#ifdef BUDDY

typedef enum
{
    FREE,
    USED,
    DIVIDED
} BState;

typedef struct BNode
{
    struct BNode *left;
    struct BNode *right;
    void *memDir;
    size_t size;
    size_t level;
    BState state;
} BNode;

typedef struct memoryManagerCDT
{
    BNode *root;
    void *memoryDir;
    void *nextBNodePos;
    size_t memorySize;
    size_t usedSize;
} memoryManagerCDT;

typedef memoryManagerCDT* memoryManagerADT;

memoryManagerADT mm;

static BNode *newBNode(memoryManagerADT mm, size_t size, void *memDir, size_t level)
{
    BNode *aux = mm->nextBNodePos;
    mm->nextBNodePos = (void *)((char *)mm->nextBNodePos + sizeof(BNode));
    aux->size = size; 
    aux->left = aux->right = NULL;
    aux->memDir = memDir; 
    aux->state = FREE;
    aux->level = level; 
    return aux;
}



void initMemMan(void *heapBase, size_t heapSize)
{
    if (heapSize < MIN_BLOCK || heapSize % 2 != 0)
    {
        return NULL;
    }
    mm = (void *)STRUCT_POS;
    mm->usedSize = 0;
    mm->memorySize = heapSize;
    mm->memoryDir = heapBase;
    mm->root = (void *)((char *)mm + sizeof(memoryManagerCDT));
    mm->nextBNodePos = (void *)((char *)mm->root + sizeof(BNode));
    mm->root->left = NULL;
    mm->root->right = NULL;
    mm->root->state = FREE;
    mm->root->memDir = mm->memoryDir;
    mm->root->level = 0;
    mm->root->size = heapSize;
}

static void *allocRecursive(BNode *current, size_t size, memoryManagerADT mm)
{
    void *toReturn = NULL;

    if (current->state == USED)
    {
        return toReturn;
    }

    else if (current->state == DIVIDED)
    {
        toReturn = allocRecursive(current->left, size, mm);
        if (toReturn == NULL)
        {
            return allocRecursive(current->right, size, mm);
        }
        return toReturn;
    }

    else if (current->state == FREE && size <= current->size)
    {
        if (current->size == MIN_BLOCK)
        {
            current->state = USED;
            mm->usedSize += current->size;
            return current->memDir;
        }

        else if (size <= current->size / 2)
        {
            if (current->left == NULL)
            {
                current->left = newBNode(mm, current->size / 2, current->memDir, current->level + 1);
            }
            if (current->right == NULL)
            {
                current->right = newBNode(mm, current->size / 2, (void *)((char *)current->memDir + current->size / 2), current->level + 1);
            }
            current->state = DIVIDED;
            return allocRecursive(current->left, size, mm);
        }

        else
        {
            current->state = USED;
            mm->usedSize += current->size;
            return current->memDir;
        }
    }

    return NULL;
}

void *malloc( uint64_t nBytes)
{
    size_t size = (size_t)nBytes;
    void *toReturn = allocRecursive(mm->root, size, mm);
    return toReturn;
}

static int buddyFreeMemoryRec(memoryManagerADT mm, void *p, BNode *node)
{
    int toReturn;
    if (node == NULL)
    {
        return 0;
    }
    if (node->state == FREE)
    {
        return 0;
    }
    else if (node->state == DIVIDED)
    {
        toReturn = buddyFreeMemoryRec(mm, p, node->left);
        if (toReturn == 0)
        {
            toReturn = buddyFreeMemoryRec(mm, p, node->right);
        }
        if (node->left->state == FREE && node->right->state == FREE)
        {
            node->state = FREE;
        }
        return toReturn;
    }
    else
    {
        if (node->memDir == p)
        {
            node->state = FREE;
            mm->usedSize -= node->size;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

void free(void *block){
    buddyFreeMemoryRec(mm, block, mm->root);
}

void memoryInformation(memInfo * m){
    m->allocatedBytes = mm->usedSize;
    m->availableBytes = mm->memorySize - mm->usedSize;
    m->totalMemory = mm->memorySize;
}

#endif
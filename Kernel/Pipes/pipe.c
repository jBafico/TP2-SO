// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <semaphores.h>
#include <lib.h>
#include <memManager.h>

#define PIPESIZE 512

typedef struct pipe {
    int id; // id of the pipe
    char data[PIPESIZE];
    uint32_t readIndex;     // number of bytes read
    uint32_t writeIndex;    // number of bytes written
    int readLock;   // read fd is still open
    int writeLock;  // write fd is still open
    int processesUsing;
}pipe;

typedef struct pipeNode {
    pipe *p;
    struct pipeNode *next;
}pipeNode;

typedef struct pipeList {
    pipeNode * first;
    pipeNode * last;
    uint32_t size;
}pipeList;

static pipeList * pipes;

int currentSemId = 0;

pipe * getPipe(int pipeID){
    if(pipes == NULL)
        return NULL;

    pipeNode * aux = pipes->first;

    while(aux != NULL){
        if(aux->p->id == pipeID)
            return aux->p;
        aux = aux->next;
    }
    return NULL;
}

pipe * createPipe(int pipeId) {
    pipe * p;

    if ((p = malloc(sizeof(pipe))) == NULL)
        return NULL;

    p->id = pipeId;
    p->writeIndex = 0;
    p->readIndex = 0;
    p->processesUsing = 0;

    if((p->readLock = semOpen(currentSemId++, 0)) == ERROR){
        free(p);
        return NULL;
    }

    if((p->writeLock = semOpen(currentSemId++, PIPESIZE)) == ERROR){
        semClose(p->readLock);
        free(p);
        return NULL;
    }

    pipeNode *node;

    if((node = malloc(sizeof(pipeNode))) == NULL){
        semClose(p->readLock);
        semClose(p->writeLock);
        free(p);
        return NULL;
    }

    node->p = p;
    node->next = NULL;

    if (pipes == NULL) {
        if((pipes = malloc(sizeof(pipeList))) == NULL){
            semClose(p->readLock);
            semClose(p->writeLock);
            free(p);
            free(node);
            return NULL;
        }
        pipes->first = node;
        pipes->last = node;
        pipes->size = 1;
    } else {
        pipes->last->next = node;
        pipes->last = node;
        pipes->size++;
    }

    return p;
}

int pipeOpen(int pipeId){
    pipe * p;

    if ((p = getPipe(pipeId)) == NULL) {
        p = createPipe(pipeId);
        if (p == NULL)
            return ERROR;
    }
    p->processesUsing++;
    pipes->size++;
    return pipeId;
}

static void deletePipe(int pipeId){
    pipeNode * aux = pipes->first;
    pipeNode * prev = NULL;

    while(aux != NULL){
        if(aux->p->id == pipeId){
            if(prev == NULL)
                pipes->first = aux->next;
            else
                prev->next = aux->next;

            semClose(aux->p->writeLock);
            semClose(aux->p->readLock);
            free(aux->p);
            free(aux);
            pipes->size--;
            return;
        }
        prev = aux;
        aux = aux->next;
    }
}

int pipeClose(int pipeId){
    pipe * p = getPipe(pipeId);
    if (p == NULL)
        return ERROR;

    p->processesUsing--;

    if (p->processesUsing == 0)
        deletePipe(pipeId);

    return 0;
}

int pipeWrite(int pipeId, const char * src){
    pipe * p = getPipe(pipeId);
    if (p == NULL)
        return ERROR;

    int i;

    for(i = 0; src[i] != '\0'; i++){
        semWait(p->writeLock);
        p->data[p->writeIndex++] = src[i];
        p->writeIndex = p->writeIndex % PIPESIZE;
        semPost(p->readLock);
    }

    return i;
}

int pipeRead(int pipeId){
    pipe * p = getPipe(pipeId);
    if (p == NULL)
        return ERROR;

    semWait(p->readLock);

    char c = p->data[p->readIndex++];
    p->readIndex = p->readIndex % PIPESIZE;

    semPost(p->writeLock);

    return c;
}
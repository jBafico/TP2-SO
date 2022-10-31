
#include "../include/phylo.h"
#include "../include/sysCalls.h"

/* TODO esto es medio pseudocodigo, desp tenemos que el tema de las syscalls
 * voy a poner los nombres genericos de las syscalls */



/*

typedef enum {THINKING, WAITING, EATING} phyloState;
#define BACKGROUND 0
#define FOREGROUND 1

typedef struct phylo{
    int pid;
    int phyloID;
    int sem;
    phyloState state;
}phylo;

phylo *philosophers[MAX_PHYLOS];
static int philosopherCount = 0;
static int mutex;
static int tableOpen;

#define LEFT(i) (((i) + philosopherCount - 1) % philosopherCount)
#define RIGHT(i) (((i) + 1) % philosopherCount)

static void think();
static void phyloMain(int argc, char **argv);
static void takeForks(int i);
static void putForks(int i);
static void eat(int i);
static int addPhylo();
static int removePhylo();
static void printTable(int argc, char **argv);
static void phyloIntro();

void phyloProblem(int argc, char **argv) {

    philosopherCount = 0;
    tableOpen = 1;
    semOpen(MUTEX_SEM_ID, 1);

    phyloIntro();

    int i = 0;
    while (i < INITIAL_PHYLOS) {
        addPhylo();
        i++;
    }

    printf("\n For the first 2 seconds you will not be able to add philosophers, let them eat.\n\n");

    char *args[] = {"Phylo Table"};
    int tablePID = sysAddProcess(&printTable, 1, args, BACKGROUND);

    sysSleep(PHYLO_WAIT);

    printf("\n The 2 second mark has passed, you can interact with the table.\n\n");

    while (tableOpen) {
        char key = getChar();
        switch (key) {
            case 'a':
                if (addPhilo() == -1) {
                    printc(RED, "\n You cant add more philosophers to the table.\n\n");
                } else {
                    printc(GREEN, "\n One philosopher was added.\n\n");
                }
                break;
            case 'r':
                if (removePhilo() == -1) {
                    printc(BLUE, "\n You cant leave, you will be here forever.\n\n");
                } else {
                    printc(RED, "\n A philosopher left, he got tired.\n\n");
                }
                break;
            case 'f':
                printf(
                        "\n DING DING The table is closed, you cant eat anymore. \n\n");
                printf("\n Apu: Thank you, come again.\n\n");
                tableOpen = 0;
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < philosopherCount; i++) {
        sysSemClose(philosophers[i]->sem);
        sysKillProcess(philosophers[i]->pid);
        sysFree(philosophers[i]);
    }
    sysKillProcess(tablePID);
    sysSemClose(MUTEX_SEM_ID);
}

static void phyloMain(int argc, char **argv) {
    int i = strToInt(argv[1], 0);
    while (1) {
        takeForks(i);
        think();
        putForks(i);
        think();
    }
}

static int addPhylo() {
    if (philosopherCount == MAX_PHYLOS) {
        return -1;
    }

    sysSemWait(mutex);
    phylo *philosopher = sysMalloc(sizeof(t_philosofer));
    if (philosopher == NULL) {
        return -1;
    }
    philosopher->sem = sysSemOpen(PHYLO_SEM_ID + philosopherCount, 1);
    philosopher->state = THINKING;
    philosopher->phyloID = philosopherCount;

    char index[3];
    //TODO guardar en index el numero de phylo en string

    char *argv[] = {"philosopher", index};
    philosopher->pid = sysAddProcess(&phyloMain, 2, argv, BACKGROUND);

    philosophers[philosopherCount++] = philosopher;

    sysSemPost(mutex);
    return 0;
}

static int removePhylo() {
    if (philosopherCount == INITIAL_PHYLOS) {
        return -1;
    }

    sysSemWait(mutex);

    phylo *philosopher = philosophers[--philosopherCount];
    sysSemClose(philosopher->sem);
    sysKillProcess(philosopher->pid);
    sysFree(philosopher);

    sysSemPost(mutex);
    return 0;
}

static void takeForks(int i) {
    sysSemWait(mutex);
    philosophers[i]->state = WAITING;
    eat(i);
    sysSemPost(mutex);
    sysSemWait(philosophers[i]->sem);
}

static void putForks(int i) {
    sysSemWait(mutex);
    philosophers[i]->state = THINKING;
    eat(LEFT(i));
    eat(RIGHT(i));
    sysSemPost(mutex);
}

static void eat(int i) {
    if (philosophers[i]->state == WAITING && philosophers[LEFT(i)]->state != EATING && philosophers[RIGHT(i)]->state != EATING) {
        philosophers[i]->state = EATING;
        sysSemPost(philosophers[i]->sem);
    }
}

static void think() {
    sysSleep(PHYLO_WAIT);
}

static void printTable(int argc, char **argv) {
    while (tableOpen) {
        sysSemWait(mutex);
        int i;
        for (i = 0; i < philosopherCount; i++) {
            if (philosophers[i]->state == EATING) {
                putChar('E');
            } else {
                putChar('-');
            }
            putChar(' ');
        }
        putChar('\n');
        sysSemPost(mutex);
        yield();
    }
}

static void phyloIntro() {
    print("Welcome to the dining philosophers problem \n");
    print("Use A to add a philosopher \n");
    print("Use R to remove a philosopher \n");
    print("Use F to finish \n");
}
*/
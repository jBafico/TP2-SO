#include <phylo.h>
#include <sysCalls.h>
#include <library.h>

typedef enum {THINKING, WAITING, EATING} phyloState;

typedef struct phylo {
    int pid;
    int sem;
    int phyloID;
    phyloState state;
}phylo;

phylo *philosophers[MAX_PHYLOS];
static int qtyPhylos = 0;
static int mutex;
static int tableOpen;

#define LEFT(i) (((i) + qtyPhylos - 1) % qtyPhylos)
#define RIGHT(i) (((i) + 1) % qtyPhylos)

static void think();
static void takeForks(int i);
static void putForks(int i);
static void tryEat(int i);
static int addPhylo();
static int removePhylo();
static void printTable(int argc, char **argv);
static void phyloIntro();

void phyloProblem(int argc, char **argv) {
    qtyPhylos = 0;
    tableOpen = true;
    mutex = sysSemOpen(MUTEX_SEM_ID, 1);

    phyloIntro();

    for (int i = 0; i < INITIAL_PHYLOS; ++i)
        addPhylo();


    printk("\nFor the first %d seconds you will not be able to add philosophers, let them eat.\n\n", PHYLO_WAIT);

    char *args[] = {"Phylo Table"};
    int tablePID = sysAddProcess(&printTable, 1, args, false, NULL);

    sysSleep(PHYLO_WAIT);

    printk("\nThe 2 second mark has passed, you can interact with the table.\n\n");

    while (tableOpen) {
        char option = getChar();
        switch (option) {
            case 'a':
                if (addPhylo() == ERROR)
                    printErr("\nYou cant add more philosophers to the table.\n\n");
                else
                    printk("\nOne philosopher was added.\n\n");
                break;
            case 'r':
                if (removePhylo() == ERROR)
                    printErr("\n You cant leave, you will be here forever.\n\n");
                else
                    printk("\n A philosopher left, he got tired.\n\n");
                break;
            case 'f':
                printk("\n DING DING The table is closed, you cant eat anymore. \n\n");
                printk("\n Apu: Thank you, come again.\n\n");
                tableOpen = false;
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < qtyPhylos; i++) {
        sysSemClose(philosophers[i]->sem);
        sysKillProcess(philosophers[i]->pid);
        sysFree(philosophers[i]);
    }

    sysKillProcess(tablePID);
    sysSemClose(MUTEX_SEM_ID);
}

static void phyloMain(int argc, char **argv) {
    int i = myAtoi(argv[1]);
    while (1) {
        takeForks(i);
        think();
        putForks(i);
        think();
    }
}

static int addPhylo() {
    if (qtyPhylos == MAX_PHYLOS)
        return ERROR;

    sysSemWait(mutex);

    phylo *p = sysMalloc(sizeof(phylo));
    if (p == NULL)
        return ERROR;

    p->sem = sysSemOpen(PHYLO_SEM_ID + qtyPhylos, 1);
    p->state = THINKING;
    p->phyloID = qtyPhylos;

    char index[3];
    itoa(qtyPhylos, index, 10);

    char *argv[] = {"philosopher", index};
    p->pid = sysAddProcess(&phyloMain, 2, argv, false, NULL);

    philosophers[qtyPhylos++] = p;

    sysSemPost(mutex);
    return 0;
}

static int removePhylo() {
    if (qtyPhylos == INITIAL_PHYLOS)
        return ERROR;

    sysSemWait(mutex);

    phylo *p = philosophers[--qtyPhylos];

    sysSemClose(p->sem);
    sysKillProcess(p->pid);
    sysFree(p);

    sysSemPost(mutex);
    return 0;
}

static void takeForks(int i) {
    sysSemWait(mutex);
    philosophers[i]->state = WAITING;
    tryEat(i);
    sysSemPost(mutex);
    sysSemWait(philosophers[i]->sem);
}

static void putForks(int i) {
    sysSemWait(mutex);
    philosophers[i]->state = THINKING;
    tryEat(LEFT(i));
    tryEat(RIGHT(i));
    sysSemPost(mutex);
}

static void tryEat(int i) {
    if (philosophers[i]->state == WAITING &&
        philosophers[LEFT(i)]->state != EATING &&
        philosophers[RIGHT(i)]->state != EATING){
        philosophers[i]->state = EATING;
        sysSemPost(philosophers[i]->sem);
    }
}

static void think() { sysSleep(PHYLO_WAIT); }

static void printTable(int argc, char **argv) {
    while (tableOpen) {
        sysSemWait(mutex);
        int i;
        for (i = 0; i < qtyPhylos; i++) {
            if (philosophers[i]->state == EATING)
                putCharacter(STDOUT, 'E');
            else
                putCharacter(STDOUT, '-');

            putCharacter(STDOUT, ' ');
        }
        putCharacter(STDOUT, '\n');
        sysSemPost(mutex);
        sysYield();
    }
}

static void phyloIntro() {
    printk("Welcome to the dining philosophers problem \n");
    printk("Use A to add a philosopher \n");
    printk("Use R to remove a philosopher \n");
    printk("Use F to finish \n");
}

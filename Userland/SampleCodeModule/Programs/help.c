#include <library.h>

#define CANT_MESSAGES 13

void help(int argc, char ** argv){
    char* helpMsg[]={ 
    "* help - Muestra las funciones disponibles para utilizar.\n",
    "* time - Despliega en pantalla el dia y la hora del sistema.\n",
    "* mem - Imprime el estado de la memoria.\n",
    "* loop - Imprime su ID con un saludo cada una determinada cantidad de segundos.\n",
    "* kill - Mata un proceso dado su ID.\n",
    "* nice - Cambia la prioridad de un proceso dado su ID y la nueva prioridad.\n",
    "* block - Cambia el estado de un proceso entre bloqueado y listo dado su ID.\n",
    "* sem - Imprime la lista de todos los semáforos con sus propiedades.\n",
    "* cat - Imprime el stdin tal como lo recibe.\n",
    "* wc - Cuenta la cantidad de líneas del input.\n",
    "* filter - Filtra las vocales del input.\n",
    "* pipe - Imprime la lista de todos los pipes con sus propiedades.\n",
    "* phylo - Implementa el problema de los filosofos comensales.\n"};

    printk("Las funciones disponibles para utilizar son:\n");
    for (int i = 0; i < CANT_MESSAGES; i++)
        printk(helpMsg[i]);
    printk("\n");
}
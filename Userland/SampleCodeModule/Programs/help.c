// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <library.h>

#define CANT_MESSAGES 19

void help(int argc, char ** argv){
    char* helpMsg[]={ 
    "* help - Muestra las funciones disponibles para utilizar.\n",
    "* time - Despliega en pantalla el dia y la hora del sistema.\n",
    "* mem - Imprime el estado de la memoria.\n",
    "* loop - Imprime su ID con un saludo cada una determinada cantidad de segundos.\n",
    "* kill - Mata un proceso dado su ID.\n",
    "* nice - Cambia la prioridad de un proceso dado su ID y la nueva prioridad.\n",
    "* block - Cambia el estado de un proceso a bloqueado dado su ID.\n",
    "* unblock - Cambia el estado de un proceso a listo dado su ID\n",
    "* sem - Imprime la lista de todos los semaforos con sus propiedades.\n",
    "* cat - Imprime el stdin tal como lo recibe.\n",
    "* wc - Cuenta la cantidad de lineas del input.\n",
    "* filter - Filtra las vocales del input.\n",
    "* pipe - Imprime la lista de todos los pipes con sus propiedades.\n",
    "* phylo - Implementa el problema de los filosofos comensales.\n",
    "* memTest - Test para probar el manejo de memoria\n",
    "* prioTest - Test para probar el manejo de prioridades de procesos\n",
    "* processTest - Test para probar el algoritmo de scheduling de procesos\n",
    "* semSyncTest - Test para probar la sincronizacion de semaforos\n",
    "* semNoSyncTest - Test para comparar resultado sin sincronizacion de semaforos\n"};

    printk("Las funciones disponibles para utilizar son:\n");
    for (int i = 0; i < CANT_MESSAGES; i++)
        printk(helpMsg[i]);
    printk("\n");
}
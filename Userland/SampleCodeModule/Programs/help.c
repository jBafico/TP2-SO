#include <library.h>

#define CANT_MESSAGES 14

void help(uint8_t fd){
    char* helpMsg[]={ 
    "* help - Muestra las funciones disponibles para utilizar\n",
    "* divZero - Genera una excepcion de division por zero\n",
    "* invalidOpCode - Genera una excepción de Operation Code Invalido\n",
    "* inforeg - Imprime en pantalla el valor de todos los registros.\nDebera primero tocar la letra CTRL para sacar una snapshot de los registros\n",
    "* printmem - Recibe como argumento un puntero, realiza en memoria un\nvolcado de 32 bytes a partir de la direccion recibida como\nargumento y lo imprime en pantalla\n",
    "* time - Despliega en pantalla el dia y la hora del sistema\n",
    "* primos - Despliega en pantalla los numeros primos\n",
    "* fibonacci - Despliega en pantalla los numeros de la sucesion de fibonacci\n",
    "* | - El pipe se puede utilizar para poder correr dos funciones en simultaneo\n",
    "      Ejemplo: Func1 | Func2\n\n",
    "Para suspender la ejecucion de un programa SIN pipe,\nse debe presionar la tecla del 1.\n",
    "Dentro del pipe, la tecla del 1 suspende la ejecucion del \nprograma a la izquierda de la pantalla.\n",
    "Analogamente, la tecla del 2 suspende el programa de la derecha\n",
    "Tanto fuera como dentro del pipe, presionando la tecla ESC\nse cancelan todas las ejecuciones y se retorna a la SHELL\n"};

    printkfd(fd,"Las funciones disponibles para utilizar son:\n");
    for (int i = 0; i < CANT_MESSAGES; i++)
        printkfd(fd,helpMsg[i]);
}
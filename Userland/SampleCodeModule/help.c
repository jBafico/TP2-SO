#include "include/library.h"

void help(){
    char* helpMsg[]={ 
    "help - Muestra las funciones disponibles para utilizar\n",
    "divZero - Genera una excepción de división por zero\n",
    "invalidOpCode - Genera una excepción de Operation Code Invalido\n",
    "inforeg - Imprime en pantalla el valor de todos los registros\n",
    "printmem - Recibe como argumento un puntero, realiza en memoria un volcado de 32 bytes a partir de la direccion recibida como argumento y lo imprime en pantalla\n",
    "time - Despliega en pantalla el dia y la hora del sistema\n",
    "primos - Despliega en pantalla los numeros primos\n",
    "fibonacci - Despliega en pantalla los numeros de la sucesion de fibonacci\n",
    "| - El pipe se puede utilizar para poder correr dos funciones en simultaneo"};
    printk("Las funciones disponibles para utilizar son:\n");
    for (int i = 0; i < 9; i++)
    {
         printk(helpMsg[i]);
    }
    
   
}
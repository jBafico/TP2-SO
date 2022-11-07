// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <clock.h>
#include <sysCalls.h>
#include <library.h>

void time(int argc, char ** argv){
    clock time;
    sysTime(&time);
    printk("La hora y dia del sistema es: %d:%d:%d del %d/%d/%d \n", time.hours, time.minutes, time.seconds, time.dayOfMonth, time.month, time.year);
}
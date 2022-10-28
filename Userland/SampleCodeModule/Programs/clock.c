#include <stdint.h>
#include <clock.h>
#include <sysCalls.h>
#include <library.h>

void time(uint8_t fd){
    clock time;
    sysTime(&time);
    printkfd(fd,"La hora y dia del sistema es: %d:%d:%d del %d/%d/%d \n", time.hours, time.minutes, time.seconds, time.dayOfMonth, time.month, time.year);
}
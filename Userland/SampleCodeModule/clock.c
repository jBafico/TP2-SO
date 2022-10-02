#include <stdint.h>
#include "include/clock.h"
#include "include/sysCalls.h"
#include "include/library.h"

void time(){
    clock time;
    sysTime(&time);
    printk("La hora y dia del sistema es: %d:%d:%d del %d/%d/%d \n", time.hours, time.minutes, time.seconds, time.dayOfMonth, time.month, time.year);

}
#include <stdint.h>

#ifndef _TIME_H_
#define _TIME_H_

typedef struct clock{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dayOfMonth;
    uint8_t month;
    uint16_t year;
}clock;

void timer_handler();
uint64_t ticks_elapsed();
uint64_t seconds_elapsed();

#endif

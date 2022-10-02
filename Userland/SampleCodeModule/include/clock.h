#ifndef SAMPLECODEMODULE_TIME_H
#define SAMPLECODEMODULE_TIME_H

typedef struct clock{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dayOfMonth;
    uint8_t month;
    uint16_t year;
}clock;

void time();
#endif //SAMPLECODEMODULE_TIME_H
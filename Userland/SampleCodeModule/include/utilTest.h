#ifndef USERLAND_UTILTEST_H
#define USERLAND_UTILTEST_H

#include <stdint.h>
#define MINOR_WAIT 1000000
#define WAIT 10000000
#define BACKGROUND 0
#define FOREGROUND 1

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char* str);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print();

#endif //USERLAND_UTILTEST_H

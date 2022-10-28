#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
uint8_t getSeconds();
uint8_t getMins();
uint8_t getHour();
uint8_t getHourUTC();
uint8_t getKey();
uint8_t getDayOfMonth();
uint8_t getMonth();
uint8_t getYear();

void restartSCM();

uint64_t * prepareRegisters();

uint8_t getByte(uint64_t address);

#endif
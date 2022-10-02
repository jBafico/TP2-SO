#include <stdint.h>

#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#define NULL (void *) 0

enum colorAttributes{Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Pink = 13,
    Yellow = 14,
    White = 15,
    };

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncPrintAttribute(const char * string, int color, int backColor);
void ncPrintCharAttribute(char character, int chColor, int backColor);
void ncNewline();
void ncClear();
void ncScroll();

void ncDeleteChar();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);

#endif
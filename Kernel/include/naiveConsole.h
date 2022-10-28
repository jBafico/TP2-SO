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

enum fds{STDIN=0, STDOUT, STDERR, STDDER, STDIZQ, STDBOTH, STDERRDER, STDERRIZQ, STDERRBOTH};

void ncPrint(const char * string);
void ncPrintRight(const char * string);
void ncPrintLeft(const char * string);
void ncPrintAttribute(const char * string, int color, int backColor);
void ncPrintLeftAttribute(const char * string, int chColor, int backColor);
void ncPrintRightAttribute(const char * string, int chColor, int backColor);
void ncPrintBothAttribute(const char * string, int chColor, int backColor);
void ncPrintFdAttribute(uint8_t fd, const char * string , int color, int backColor);

void ncPrintChar(char character);
void ncPrintCharAttribute(char character, int chColor, int backColor);
void ncPrintCharFdAttribute(uint8_t fd, char character, int color, int backColor);
void ncPrintCharRightAttribute(char character, int chColor, int backColor);
void ncPrintCharLeftAttribute(char character, int chColor, int backColor);
void ncPrintCharBothAttribute(char character, int chColor, int backColor);

void ncNewline();
void ncNewLineFd(uint8_t fd);
void ncNewlineLeft();
void ncNewlineRight();
void ncNewlineBoth();

void ncClearFd(uint8_t fd);
void ncClear();
void ncClearLeft();
void ncClearRight();
void ncScroll();

void ncDeleteChar();

void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintHexFdAttribute(uint8_t fd, uint64_t value, int color, int backColor);
void ncPrintBase(uint64_t value, uint32_t base);
void ncPrintBaseFdAttribute(uint8_t fd, uint64_t value, uint32_t base, int color, int backColor);
void ncPrintBin(uint64_t value);


#endif
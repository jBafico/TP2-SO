#ifndef SAMPLECODEMODULE_LIBRARY_H
#define SAMPLECODEMODULE_LIBRARY_H

#include <stdarg.h>
#include <stdint.h>

#define FALSE 0
#define TRUE !FALSE

#define NULL (void *) 0

#define REGULAR 0
#define ESCAPE 1
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define BUFFSIZE 128
#define DECIMALBASE 10
#define HEXABASE 16

void printString(uint64_t fd, const char * vec);
void putCharacter(uint64_t fd, char c);
void vprintk(uint64_t fd,const char * fmt, va_list args);
void printk(const char * fmt, ...);
void printErr(const char * fmt, ...);
int strcmp(const char * str1, const char * str2);
char getChar();

#endif //SAMPLECODEMODULE_LIBRARY_H

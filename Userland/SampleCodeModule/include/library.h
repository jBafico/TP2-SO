#ifndef SAMPLECODEMODULE_LIBRARY_H
#define SAMPLECODEMODULE_LIBRARY_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ERROR (-1)
#define ADD_PROC_ERROR (-2)

#define EOF 96

#define REGULAR 0
#define ESCAPE 1

enum fds{STDIN=0, STDOUT, STDERR, STDDER, STDIZQ, STDBOTH, STDERRDER, STDERRIZQ, STDERRBOTH};
#define DISTANCE_TO_ERRORFD 3

#define BUFFSIZE 128
#define DECIMALBASE 10
#define HEXABASE 16

void printString(uint64_t fd, const char * vec);
void putCharacter(uint64_t fd, char c);
void vprintk(uint64_t fd,const char * fmt, va_list args);
void printkfd(int fd, const char * fmt, ...);
void printk(const char * fmt, ...);
void printErr(const char * fmt, ...);
int strcmp(const char * str1, const char * str2);
char* strcpy(char* dest, const char* src);
char getChar();
int getStringLength(const char *vec);
int myAtoi(char * str);
void *memset(void *destination, int32_t c, uint64_t length);
char* itoa(int value, char* buffer, int base);
int isVowel(char c);

#endif //SAMPLECODEMODULE_LIBRARY_H

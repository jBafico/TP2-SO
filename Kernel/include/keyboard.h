#include <stdint.h>

#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#define CANT_KEYS 128
#define RELEASE 0x80
#define ENTER 28
#define LSHIFT 0x2A
#define RSHIFT 0x36

#define ESCAPE 1
#define ONE 2
#define TWO 3
#define CONTROL 29
#define BACKSPACE 14


void ncClear();
void keyboard_handler();
char * getBuffer(int * writerVal);
uint8_t getCtrlFlag();

#endif //KERNEL_KEYBOARD_H

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <int80Dispatcher.h>
#include <scheduler.h>
#include <stdbool.h>

#define MAX_BUFF 512

#define STDIN 0
#define EOF 96

//Tabla para convertir lo que recibe  y en letra
static char kbd_US [CANT_KEYS] =
        {
                0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
                '\t', /* <-- Tab */
                'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
                13, /* <-- control key */
                'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
                '*',
                0,  /* Alt */
                ' ',  /* Space bar */
                0,  /* Caps lock */
                0,  /* 59 - F1 key ... > */
                0,   0,   0,   0,   0,   0,   0,   0,
                0,  /* < ... F10 */
                0,  /* 69 - Num lock*/
                0,  /* Scroll Lock */
                0,  /* Home key */
                0,  /* Up Arrow */
                0,  /* Page Up */
                '-',
                0,  /* Left Arrow */
                0,
                0,  /* Right Arrow */
                '+',
                0,  /* 79 - End key*/
                0,  /* Down Arrow */
                0,  /* Page Down */
                0,  /* Insert Key */
                0,  /* Delete Key */
                0,   0,   0,
                0,  /* F11 Key */
                0,  /* F12 Key */
                0,  /* All other keys are undefined */
        };

static char shift_kbd_US [CANT_KEYS] =
        {
                0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
                '\t', /* <-- Tab */
                'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
                0, /* <-- control key */
                'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '`',  0, '|',
                'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,
                '*',
                0,  /* Alt */
                ' ',  /* Space bar */
                0,  /* Caps lock */
                0,  /* 59 - F1 key ... > */
                0,   0,   0,   0,   0,   0,   0,   0,
                0,  /* < ... F10 */
                0,  /* 69 - Num lock*/
                0,  /* Scroll Lock */
                0,  /* Home key */
                0,  /* Up Arrow */
                0,  /* Page Up */
                '-',
                0,  /* Left Arrow */
                0,
                0,  /* Right Arrow */
                '+',
                0,  /* 79 - End key*/
                0,  /* Down Arrow */
                0,  /* Page Down */
                0,  /* Insert Key */
                0,  /* Delete Key */
                0,   0,   0,
                0,  /* F11 Key */
                0,  /* F12 Key */
                0,  /* All other keys are undefined */
        };

char keyboardBuffer[MAX_BUFF];
uint16_t writer = 0;

void ncPrintKey(uint16_t code){
    if(code < CANT_KEYS) {
        char toPrint = kbd_US[code];
        if (toPrint != 0)
            ncPrintChar(toPrint);
    }
}

void ncPrintKeyShift(uint16_t code){
    if(code < CANT_KEYS) {
        char toPrint = shift_kbd_US[code];
        if (toPrint != 0)
            ncPrintChar(toPrint);
    }
}

static uint8_t shiftFlag = false;
static uint8_t ctrlFlag = false;

void keyboard_handler(){
    uint16_t teclahex = getKey();
    if(teclahex == RSHIFT || teclahex == LSHIFT)
        shiftFlag = true;
    else if(teclahex == (RSHIFT + RELEASE) || teclahex == (LSHIFT + RELEASE)) //Ambos release shifts del teclado
        shiftFlag = false;
    else if(teclahex == CONTROL)
        ctrlFlag = true;
    else if(teclahex == (CONTROL + RELEASE))
        ctrlFlag = false;
    else if(teclahex == ESCAPE)
        killCurrentFGProcess();
    else if (teclahex < RELEASE) {
        if(kbd_US[teclahex] != 0) {
            if(!ctrlFlag) {
                if (shiftFlag == 0)
                    keyboardBuffer[writer++] = kbd_US[teclahex];
                else
                    keyboardBuffer[writer++] = shift_kbd_US[teclahex];
            }
            else if(kbd_US[teclahex] == 'd')
                keyboardBuffer[writer++] = EOF;
        }
    }
    writer %= MAX_BUFF;
}
  

char * getBuffer(int * writerVal){
    *writerVal = writer;
    return keyboardBuffer;
}






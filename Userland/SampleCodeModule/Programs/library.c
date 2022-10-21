#include "library.h"
#include "sysCalls.h"

int strcmp(const char * str1, const char * str2){
   while(*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

int abs(int num){
    return num >= 0 ? num: -1 * num;
}

// Function to swap two numbers
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }

    return buffer;
}

// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }

    // consider the absolute value of the number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }

        n = n / base;
    }

    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }

    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

void putCharacter(uint64_t fd, char c){
    sysWrite(fd,&c,1);
}

int getStringLength(const char *vec) {
    int i = 0;
    while ( vec[i] != 0)
        i++;
    return i;
}

void printString(uint64_t fd, const char * vec){
    int length = getStringLength(vec);
    sysWrite(fd,vec,length);
}

void vprintk(uint64_t fd, const char * fmt, va_list args){
    char buffer[BUFFSIZE];
    int state = REGULAR;

    while (*fmt) {
        if ( state == REGULAR ){
            if ( *fmt == '%' ){
                state = ESCAPE;
            } else {
                //si estoy en regular y no es caracter especial, entonces lo imprimo
                putCharacter(fd,*fmt);
            }
            //si estoy en el otro estado
        } else {
            switch (*fmt)
            {
                case 'c':
                    putCharacter(fd,va_arg(args, int));
                    break;
                case 'd':
                    printString( fd , itoa(va_arg(args,uint32_t),buffer,DECIMALBASE) );
                    break;
                case 'f':
                    break;
                case 's':
                    printString(fd,va_arg(args, const char *));
                    break;
                case 'x':
                case 'p':
                    putCharacter(fd,'0');
                    putCharacter(fd,'X');
                    printString(fd, itoa( va_arg(args, uint64_t) ,buffer, HEXABASE));
                    break;
                default:
                    break;
            }
            state = REGULAR;
        }
        fmt++;
    }
}

void printk(const char * fmt, ...){
    va_list args;
    va_start(args,fmt);
    vprintk(STDOUT,fmt,args);
    va_end(args);
}

void printErr(const char * fmt, ...){
    va_list args;
    va_start(args,fmt);
    vprintk(STDERR,fmt,args);
    va_end(args);
}


char getChar(){
    char c = 0;
    uint16_t ret = 0;
    do{
        ret = sysRead(STDIN,&c,1);
    } while (ret != 1);
    //TODO VER ESTA CONDICION DE CORTE, ME PARECE QUE ESTA MAL
    return c;
}
#include <printMem.h>
#include <library.h>
#include <sysCalls.h>

#define BUFFSIZE 128

#define ERROR 0
#define NOERROR 1


// Obtenido de https://github.com/dart-lang/sdk/issues/2624 y adaptada para que funcione en C
static uint64_t hexToNum(char * hex, int * error){
    if ( hex == NULL || hex[0] != '0' || hex[1] != 'x' ){
        *error = ERROR;
        return 0;
    }

    int i = 2;

    uint64_t val = 0;

    int len = getStringLength(hex);
    for (; i < len; i++)
    {
        int hexDigit = hex[i];
        if (hexDigit >= 48 && hexDigit <= 57)
            val += (hexDigit - 48) * (1 << (4 * (len - 1 - i)));
        else if (hexDigit >= 65 && hexDigit <= 70)
            val += (hexDigit - 55) * (1 << (4 * (len - 1 - i))); // A..F
        else if (hexDigit >= 97 && hexDigit <= 102)
            val += (hexDigit - 87) * (1 << (4 * (len - 1 - i))); // a..f
        else{
            *error = ERROR;
            return 0;
        }
    }
    *error = NOERROR;
    return val;
}


void printMem(char * str, uint8_t fd){
    uint64_t address;

    int controlFlag;
    address = hexToNum(str,&controlFlag);

    if (controlFlag == ERROR){
        if(fd==STDOUT)
            fd = STDERR;
        else
            fd+=DISTANCE_TO_ERRORFD;

        printkfd(fd,"Argumento Invalido");
        return;
    }

    uint8_t mem[BUFFSIZE];

    if(sysPrintMem(mem, address)==-1){
        if(fd==STDOUT)
            fd = STDERR;
        else
            fd+=DISTANCE_TO_ERRORFD;

        printkfd(fd, "Direccion invalida\n");
    }else{
        for(int t=0; t<32; t++) {
            printkfd(fd,"%x ", (int) mem[t]);
        }
        printkfd(fd,"\n");
    }

}


#include "include/printMem.h"
#include "include/library.h"
#include <stdint.h>


void printMem(char* str){
    uint64_t address;
    int len = getStringLength(str);
    //chequeo si address cumple con 0x12345678h (0x[0-9a-f]{8}h)??
    if ( len<3 || len >11 || str[0]!='0' || str[1]!='x'){
        printk("dirección invalida");
        return 1;
    }
    //transformo el string en hexa a un int para pasarselo a sys_mem
    for(int i=2; i<len; i++){
		if(str[i]>='0' && str[i]<='9')
			address = 16*address + str[i]-'0';
		else if(str[i]>='a' && str[i]<='f')
			address = 16*address + str[i]-'a';
		else{
            printk("dirección invalida");
            return 1;
        }
	}
    char* mem;
    sysPrintMem(&mem, address);
    for(int i=0; i<32; i++) {
		printk("%x ", mem[i]);
	}
    printk("\n");
}


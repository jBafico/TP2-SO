#include "include/primeNums.h"
#include "include/library.h"
#include <stdint.h>

int isPrime(uint64_t number) {
    for (uint64_t i=2; i<number/2; i++) {
        if (number % i == 0 && i != number)
            return 0;
    }
    return 1;
}

void primeNumbers(){
    printk("Prime Numbers: \n");

    /* Find all Prime numbers between 1 to end */
    uint64_t i = 2;
    while (1)
    {
        if(isPrime(i))
            printk("%d\n", i);
        i++;
    }
}

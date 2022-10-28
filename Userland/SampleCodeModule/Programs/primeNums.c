#include <primeNums.h>
#include <library.h>
#include <sysCalls.h>

static uint64_t current = 2;

int isPrime(uint64_t number) {
    for(uint64_t i=2;i<=number/2;i++){
        if(number%i==0)
            return FALSE;
    }
    return TRUE;
}

void nextPrime(uint8_t fd){
    if(current == 2)
        printkfd(fd, "Prime Numbers: \n");

    if(isPrime(current)){
        printkfd(fd, "%d\n", current);
        sysSleep(1);
    }
    current++;
    while (!isPrime(current))
        current++;
}

void restartPrimes(){
    current=2;
}

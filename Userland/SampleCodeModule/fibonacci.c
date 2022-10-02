#include "fibonacci.h"
#include "library.h"
#include <stdint.h>

#define FIBO0 0
#define FIBO1 1

void fibo(){
    // initialize first and second terms
    uint64_t t1 = FIBO0, t2 = FIBO1;

    // initialize the next term (3rd term)
    uint64_t nextTerm = t1 + t2;

    // print the first two terms t1 and t2
    printk("Fibonacci Series:\nFibo(0) = %d\nFibo(1) = %d\n",t1,t2);

    // print 3rd to nth terms
    uint32_t i=2;
    while(1){
        printk("Fibo(%d) = %d\n", i, nextTerm);
        t1 = t2;
        t2 = nextTerm;
        nextTerm = t1 + t2;
        i++;
    }
}

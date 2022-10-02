#include <time.h>
#include <naiveConsole.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
    if(seconds_elapsed() == 5){
        //ncPrint("Pasaron 5 segundos");
        //ncNewline();
        ticks = 0;
    }
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

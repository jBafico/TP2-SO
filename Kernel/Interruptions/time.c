// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>

static uint64_t ticks = 0;

void timer_handler() {
	ticks++;
}

uint64_t ticks_elapsed(){
	return ticks;
}

uint64_t seconds_elapsed() {
	return ticks / 18;
}

#include "clock.h"
#include "platform.h"

void clock_set(clock* clk) {
    clk->start_time = platform_time();
    clk->elapsed = 0;
}

void clock_update(clock* clk) {
    clk->elapsed = platform_time() - clk->start_time;
}
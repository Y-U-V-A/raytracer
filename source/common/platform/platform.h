#ifndef PLATFORM__H
#define PLATFORM__H

#include "defines.h"

void platform_sleep(u64 ms);

f64 platform_time();

u32 platform_get_processor_count();

#endif
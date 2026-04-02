#ifndef ZTHREAD__H
#define ZTHREAD__H

#include "defines.h"

typedef struct zthread {
    void* internal_data;
} zthread;

#ifdef PLATFORM_WINDOWS
typedef unsigned long zthread_func_return_type;
#else
typedef void* zthread_fun_return_type;
#endif

typedef zthread_func_return_type (*PFN_zthread_start_func)(void*);

bool zthread_create(PFN_zthread_start_func func, void* params, zthread* out_thread);

void zthread_destroy(zthread* thread);

bool zthread_wait(zthread* thread);

bool zthread_wait_on_all(zthread* threads, u32 count);

#endif
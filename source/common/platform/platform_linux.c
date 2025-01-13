#include "platform.h"

#ifdef PLATFORM_LINUX

#    include <pthread.h>
#    include <unistd.h>
#    include <stdlib.h>
#    include <time.h>
#    include <sys/sysinfo.h>
#    include <semaphore.h>
#    include "zmemory.h"
#    include "logger.h"
#    include "zthread.h"
#    include "zmutex.h"
// use - lrt(real time library) while linking

void platform_sleep(u64 ms) {
    usleep(ms);
}

f64 platform_time() {
    struct timespec curr_time;
    clock_gettime(CLOCK_MONOTONIC, &curr_time);
    return (f64)curr_time.tv_sec + curr_time.tv_nsec / 1e9;
}

u32 platform_get_processor_count(void) {
    // Load processor info.
    i32 processor_count = get_nprocs_conf();
    i32 processors_available = get_nprocs();
    LOGI("%i processor cores detected, %i cores available.", processor_count, processors_available);
    return processors_available;
}

bool zthread_create(PFN_zthread_start_func func, void* params, zthread* out_thread) {
    if (0 == func || 0 == params || 0 == out_thread) {
        LOGE("zthread_create: invalid params");
        return false;
    }
    if (0 != pthread_create((pthread_t*)out_thread, 0, func, params)) {
        LOGE("zthread_create: failed to create thread");
        return false;
    }
    return true;
}

void zthread_destroy(zthread* thread) {
    if (0 == thread) {
        LOGE("zthread_destroy: invalid params");
        return;
    }
}

bool zthread_wait(zthread* thread) {
    if (0 == thread) {
        LOGE("zthread_wait: invalid params");
        return false;
    }

    if (0 != pthread_join((pthread_t)thread->internal_data, 0)) {
        LOGW("zthread_wait: failed to join");
        return false;
    }

    return true;
}

bool zthread_wait_on_all(zthread* threads, u32 count) {
    if (0 == threads || 0 == count) {
        LOGE("zthread_wait_on_all: invalid params");
        return false;
    }

    for (u32 i = 0; i < count; ++i) {
        if (0 != pthread_join((pthread_t)threads[i].internal_data, 0)) {
            LOGW("zthread_wait_on_all: failed to join");
            return false;
        }
    }

    return true;
}

bool zmutex_create(zmutex* out_mutex) {

    if (0 == out_mutex) {
        LOGE("zmutex_create: invalid params");
        return false;
    }

    out_mutex->internal_data = malloc(sizeof(pthread_mutex_t));

    if (0 != pthread_mutex_init(out_mutex->internal_data, 0)) {
        LOGE("zmutex_create : failed to create zmutex");
        return false;
    }

    return true;
}

void zmutex_destroy(zmutex* mutex) {
    if (0 == mutex) {
        LOGE("zmutex_destroy: invalid params");
        return;
    }
    if (0 != pthread_mutex_destroy(mutex->internal_data)) {
        LOGE("zmutex_destroy : failed to destroy zmutex");
        return;
    }
    free(mutex->internal_data);
}

bool zmutex_lock(zmutex* mutex) {
    if (0 == mutex) {
        LOGE("zmutex_lock: invalid params");
        return false;
    }
    // if mutex==false then mutex=true and continue executing thread
    // else blocks the thread
    if (0 != pthread_mutex_lock(mutex->internal_data)) {
        LOGE("zmutex_lock : timed out");
        return false;
    }
    return true;
}

bool zmutex_unlock(zmutex* mutex) {
    if (0 == mutex) {
        LOGE("zmutex_unlock: invalid params");
        return false;
    }
    // if mutex==true then mutex=false
    // else undefined error
    if (0 != pthread_mutex_unlock(mutex->internal_data)) {
        LOGE("zmutex_unlock : failed to unlock zmutex");
        return false;
    }
    return true;
}

#endif
#include "platform.h"

#ifdef PLATFORM_WINDOWS

#    include <Windows.h>
#    include "zthread.h"
#    include "zmutex.h"
#    include "logger.h"

void platform_sleep(u64 ms) {
    Sleep(ms);
}

f64 platform_time() {
    LARGE_INTEGER curr_ticks;
    QueryPerformanceCounter(&curr_ticks);
    LARGE_INTEGER ticks_per_sec;
    QueryPerformanceFrequency(&ticks_per_sec);
    return curr_ticks.QuadPart / (f64)ticks_per_sec.QuadPart;
}

u32 platform_get_processor_count() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    LOGI("%i processor cores detected.", info.dwNumberOfProcessors);
    return info.dwNumberOfProcessors;
}

bool zthread_create(PFN_zthread_start_func func, void* params, zthread* out_thread) {
    if (0 == func || 0 == params || 0 == out_thread) {
        LOGE("zthread_create: invalid params");
        return false;
    }
    out_thread->internal_data = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)func, params, 0, 0);
    if (0 == out_thread->internal_data) {
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

    if (!CloseHandle(thread->internal_data)) {
        LOGE("zthread_destroy: failed to close handle");
    }
}

bool zthread_wait(zthread* thread) {
    if (0 == thread) {
        LOGE("zthread_wait: invalid params");
        return false;
    }

    DWORD result = WaitForSingleObject(thread->internal_data, INFINITE);
    if (WAIT_ABANDONED == result || WAIT_TIMEOUT == result || WAIT_FAILED == result) {
        LOGE("zthread_wait: wait failed");
        return false;
    }

    return true;
}

bool zthread_wait_on_all(zthread* threads, u32 count) {
    if (0 == threads || 0 == count) {
        LOGE("zthread_wait_on_all: invalid params");
        return false;
    }

    DWORD result = WaitForMultipleObjects(count, (HANDLE*)threads, TRUE, INFINITE);
    if (WAIT_TIMEOUT == result || WAIT_FAILED == result) {
        LOGE("zthread_wait: wait failed");
        return false;
    }
    return true;
}

bool zmutex_create(zmutex* out_mutex) {

    if (0 == out_mutex) {
        LOGE("zmutex_create: invalid params");
        return false;
    }
    out_mutex->internal_data = CreateMutex(0, FALSE, 0);
    return out_mutex->internal_data != 0;
}

void zmutex_destroy(zmutex* mutex) {
    if (0 == mutex) {
        LOGE("zmutex_destroy: invalid params");
        return;
    }
    if (!CloseHandle(mutex->internal_data)) {
        LOGE("zmutex_destroy: failed to close handle");
    }
}

bool zmutex_lock(zmutex* mutex) {
    if (0 == mutex) {
        LOGE("zmutex_lock: invalid params");
        return false;
    }
    // if mutex is signaled then the mutex is unsignaled and the thread will enter
    // else thread will wait until the mutex is signaled
    DWORD result = WaitForSingleObject(mutex->internal_data, INFINITE);

    if (WAIT_ABANDONED == result || WAIT_TIMEOUT == result || WAIT_FAILED == result) {
        LOGE("zmutex_lock: wait failed");
        return false;
    }
    return true;
}

bool zmutex_unlock(zmutex* mutex) {
    if (0 == mutex) {
        LOGE("zmutex_unlock: invalid params");
        return false;
    }
    // mutex is signaled;
    if (!ReleaseMutex(mutex->internal_data)) {
        LOGE("zmutex_unlock: failed");
        return false;
    }
    return true;
}

#endif
#include "zmemory.h"
#include <stdlib.h>
#include <string.h>
#include "zmutex.h"
#include "logger.h"

typedef struct zmemory_state {
    u64 allocated_memory;
    zmutex mutex;
} zmemory_state;

static zmemory_state* state_ptr;

bool zmemory_init() {
    if (state_ptr) {
        LOGE("zmemory_init: called too many times");
        return false;
    }

    state_ptr = malloc(sizeof(zmemory_state));
    state_ptr->allocated_memory = 0;
    if (!zmutex_create(&state_ptr->mutex)) {
        LOGE("zmemory_init: failed to create mutex");
        return false;
    }
    return true;
}

void zmemory_destroy() {
    if (state_ptr) {
        zmutex_destroy(&state_ptr->mutex);
        free(state_ptr);
        state_ptr = 0;
    }
}

void* zmemory_allocate(u64 size) {
    if (0 == size) {
        LOGE("zmemory_allocate : invalid params");
        return 0;
    }

    zmutex_lock(&state_ptr->mutex);
    void* block = malloc(size);
    memset(block, 0, size);
    state_ptr->allocated_memory += size;
    zmutex_unlock(&state_ptr->mutex);

    if (0 == block) {
        LOGE("zmemory_allocate : malloc failed to allocate");
        return 0;
    }
    return block;
}

void zmemory_free(void* block, u64 size) {
    if (0 == block) {
        LOGE("zmemory_free: invalid params");
        return;
    }

    zmutex_lock(&state_ptr->mutex);
    free(block);
    state_ptr->allocated_memory -= size;
    zmutex_unlock(&state_ptr->mutex);
}

void* zmemory_set(void* block, i32 value, u64 size) {
    return memset(block, value, size);
}

void* zmemory_set_zero(void* block, u64 size) {
    return memset(block, 0, size);
}

void* zmemory_copy(void* dest, const void* src, u64 size) {
    return memcpy(dest, src, size);
}

void* zmemory_move(void* dest, const void* src, u64 size) {
    return memmove(dest, src, size);
}

i32 zmemory_compare(const void* block1, const void* block2, u64 size) {
    return memcmp(block1, block2, size);
}

void zmemory_log() {
    LOGD("current heap allocated memory = %llu", state_ptr->allocated_memory);
}
#include "darray.h"
#include "zmemory.h"
#include "logger.h"

/////////////////////////////////////////////////////////////////
//        __                                                   //
//       /  |                                                  //
//   ____$$ |  ______    ______    ______   ______   __    __  //
//  /    $$ | /      \  /      \  /      \ /      \ /  |  /  | //
// /$$$$$$$ | $$$$$$  |/$$$$$$  |/$$$$$$  |$$$$$$  |$$ |  $$ | //
// $$ |  $$ | /    $$ |$$ |  $$/ $$ |  $$/ /    $$ |$$ |  $$ | //
// $$ \__$$ |/$$$$$$$ |$$ |      $$ |     /$$$$$$$ |$$ \__$$ | //
// $$    $$ |$$    $$ |$$ |      $$ |     $$    $$ |$$    $$ | //
//  $$$$$$$/  $$$$$$$/ $$/       $$/       $$$$$$$/  $$$$$$$ | //
//                                                  /  \__$$ | //
//                                                  $$    $$/  //
//                                                   $$$$$$/   //
//                                                             //
/////////////////////////////////////////////////////////////////

// TODO: add shrink_to_fit

#define DARRAY_GET_BLOCK(array) ((u64*)array - DARRAY_FIELD_MAX)
#define DARRAY_HEADER_SIZE sizeof(u64) * DARRAY_FIELD_MAX

void* _darray_create(u64 length, u64 stride, const char* file, i32 line) {
    if (!length || !stride) {
        LOGE("_darray_create: invalid params");
        return 0;
    }
    u64 total = DARRAY_HEADER_SIZE + length * stride;
    u64* block = zmemory_allocate(total);
    block[DARRAY_FIELD_LENGTH] = 0;
    block[DARRAY_FIELD_STRIDE] = stride;
    block[DARRAY_FIELD_CAPACITY] = length;
    return block + DARRAY_FIELD_MAX;
}

void darray_destroy(void* array) {
    if (!array) {
        LOGE("darray_destroy: invalid params");
        return;
    }
    u64* block = DARRAY_GET_BLOCK(array);
    zmemory_free(block, DARRAY_HEADER_SIZE + block[DARRAY_FIELD_CAPACITY] * block[DARRAY_FIELD_STRIDE]);
}

u64 darray_field(void* array, darray_header field) {
    if (!array || field >= DARRAY_FIELD_MAX) {
        LOGE("darray_field: invalid params");
        return 0;
    }
    u64* block = DARRAY_GET_BLOCK(array);
    return block[field];
}

void* _darray_resize(void* array) {
    if (!array) {
        LOGE("darray_resize: invalid params");
        return 0;
    }
    u64* block = DARRAY_GET_BLOCK(array);
    u64 current_data_size = block[DARRAY_FIELD_STRIDE] * block[DARRAY_FIELD_CAPACITY];
    u64 new_data_size = current_data_size * DARRAY_RESIZE_FACTOR;
    // Realloc with both header and new data size
    block = zmemory_reallocate(block, new_data_size, current_data_size);
    block[DARRAY_FIELD_CAPACITY] *= DARRAY_RESIZE_FACTOR;

    return block ? (block + DARRAY_FIELD_MAX) : 0;
}

void* _darray_push_back(void* array, void* data) {
    if (!array) {
        LOGE("darray_push_back: invalid params");
        return 0;
    }
    u64 length = darray_length(array);
    u64 cap = darray_capacity(array);
    if (length == cap) {
        darray_resize(array);
    }
    u64* block = DARRAY_GET_BLOCK(array);
    length = darray_length(array);
    cap = darray_capacity(array);
    zmemory_copy((void*)((u64)array + (block[DARRAY_FIELD_LENGTH] * block[DARRAY_FIELD_STRIDE])), data, block[DARRAY_FIELD_STRIDE]);
    block[DARRAY_FIELD_LENGTH] += 1;
    return array;
}

void darray_pop_back(void* array) {
    if (!array) {
        LOGE("darray_pop_back: invalid params");
        return;
    }
    u64* block = DARRAY_GET_BLOCK(array);
    if (block[DARRAY_FIELD_LENGTH] == 0) {
        LOGE("darray_pop_back: array is empty !");
        return;
    }
    block[DARRAY_FIELD_LENGTH] -= 1;
}

void* _darray_insert(void* array, u64 index, void* data) {
    if (!array) {
        LOGE("darray_insert: invalid params");
        return 0;
    }
    u64 length = darray_length(array);
    u64 cap = darray_capacity(array);
    if (index >= length) {
        LOGE("darray_insert: index out of bounds");
        return array;
    }
    if (length == cap) {
        darray_resize(array);
    }
    u64* block = DARRAY_GET_BLOCK(array);
    u64 stride = block[DARRAY_FIELD_STRIDE];
    u64 addr = (u64)array;
    zmemory_move((void*)(addr + (index + 1) * stride),
                 (void*)(addr + index * stride),
                 (block[DARRAY_FIELD_LENGTH] - index - 1) * stride);
    zmemory_copy((void*)(addr + index * stride), data, stride);
    block[DARRAY_FIELD_LENGTH] += 1;
    return array;
}

void darray_remove(void* array, u64 index) {
    if (!array) {
        LOGE("darray_remove: invalid params");
        return;
    }
    u64* block = DARRAY_GET_BLOCK(array);
    if (index >= block[DARRAY_FIELD_LENGTH]) {
        LOGE("darray_remove: index out of bounds");
        return;
    }
    u64 stride = block[DARRAY_FIELD_STRIDE];
    u64 addr = (u64)array;
    if (index != block[DARRAY_FIELD_LENGTH] - 1) {
        zmemory_move((void*)(addr + index * stride),
                     (void*)(addr + (index + 1) * stride),
                     (block[DARRAY_FIELD_LENGTH] - index - 1) * stride);
    }
    block[DARRAY_FIELD_LENGTH] -= 1;
}

void darray_clear(void* array) {
    if (!array) {
        LOGE("darray_clear: invalid params");
        return;
    }
    u64* block = DARRAY_GET_BLOCK(array);
    block[DARRAY_FIELD_LENGTH] = 0;
}
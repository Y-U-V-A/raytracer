#ifndef DARRAY__H
#define DARRAY__H

#include "defines.h"

typedef enum darray_header {
    DARRAY_FIELD_LENGTH,
    DARRAY_FIELD_STRIDE,
    DARRAY_FIELD_CAPACITY,
    DARRAY_FIELD_MAX,
} darray_header;

#define DARRAY_DEFAULT_LENGTH 4
#define DARRAY_RESIZE_FACTOR 2
#define darray_create(type) _darray_create(DARRAY_DEFAULT_LENGTH, sizeof(type), __FILE__, __LINE__)
#define darray_reserve(length, type) _darray_create(length, sizeof(type), __FILE__, __LINE__)
#define darray_resize(array) array = _darray_resize(array)
#define darray_length(array) darray_field(array, DARRAY_FIELD_LENGTH)
#define darray_stride(array) darray_field(array, DARRAY_FIELD_STRIDE)
#define darray_capacity(array) darray_field(array, DARRAY_FIELD_CAPACITY)
#define darray_push_back(array, data)                                  \
    {                                                                  \
        typeof(data) GLUE(__darray__, __LINE__) = data;                \
        array = _darray_push_back(array, &GLUE(__darray__, __LINE__)); \
    }
#define darray_insert(array, index, data)                                  \
    {                                                                      \
        typeof(data) GLUE(__darray__, __LINE__) = data;                    \
        array = _darray_insert(array, index, &GLUE(__darray__, __LINE__)); \
    }

void* _darray_create(u64 length, u64 stride, const char* file, i32 line);

void darray_destroy(void* array);

u64 darray_field(void* array, darray_header field);

void* _darray_resize(void* array);

void* _darray_push_back(void* array, void* data);

void darray_pop_back(void* array);

void* _darray_insert(void* array, u64 index, void* data);

void darray_remove(void* array, u64 index);

void darray_clear(void* array);

#endif
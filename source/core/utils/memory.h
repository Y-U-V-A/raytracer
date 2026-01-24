#ifndef MEMORY__H
#define MEMORY__H

#include "defines.h"

#define malloc(size) memory_allocate(size)
#define free(block,size)  memory_free(block,size)

void*memory_allocate(u64 size);

void memory_free(void*block,u64 size);

#endif
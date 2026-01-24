#include "memory.h"
#undef malloc
#undef free
#include <stdlib.h>

u64 memory;

void*memory_allocate(u64 size){
	memory+=size;
	return malloc(size);
}

void memory_free(void*block,u64 size){
	memory-=size;
	free(block);
}
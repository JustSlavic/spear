#ifndef _SPEAR_ALLOCATOR_H
#define _SPEAR_ALLOCATOR_H

#include <corelibs/base.h>

typedef void *memory_allocator;

memory_allocator memory_allocator_arena_create(void *memory, uint64 size, char const *allocator_name);
memory_allocator memory_allocator_malloc_create(void);

void *memory_allocator_allocate_(memory_allocator a, uint64 size, uint64 alignment, code_location cl);
void *memory_allocator_allocate(memory_allocator a, uint64 size, uint64 alignment, code_location cl);
void memory_allocator_deallocate(memory_allocator a, void *memory, code_location cl);
void memory_allocator_arena_reset(memory_allocator a);

#define ALLOCATE_(A, T) memory_allocator_allocate_(A, sizeof(T), alignof(T), CL_HERE)
#define ALLOCATE(A, T)  memory_allocator_allocate(A, sizeof(T), alignof(T), CL_HERE)
#define ALLOCATE_BUFFER_(A, SIZE) memory_allocator_allocate_(A, SIZE, 8, CL_HERE)
#define ALLOCATE_BUFFER(A, SIZE)  memory_allocator_allocate(A, SIZE, 8, CL_HERE)
#define ALLOCATE_ARRAY_(A, T, COUNT) memory_allocator_allocate_(A, sizeof(T) * (COUNT), alignof(T), CL_HERE)
#define ALLOCATE_ARRAY(A, T, COUNT)  memory_allocator_allocate(A, sizeof(T) * (COUNT), alignof(T), CL_HERE)
#define DEALLOCATE(A, P) memory_allocator_deallocate(A, P, CL_HERE)


#endif // _SPEAR_ALLOCATOR_H

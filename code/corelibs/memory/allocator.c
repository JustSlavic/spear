#include "allocator.h"

#include <stdlib.h>
#include <string.h>


typedef enum
{
    MemoryAllocator_Invalid = 0,

    MemoryAllocator_Arena,
    MemoryAllocator_Malloc,
    MemoryAllocator_OS,
} memory_allocator_tag;

typedef struct
{
    memory_allocator_tag tag;
    uint64 size;
    uint64 used;
    char const *name;
} memory_allocator_arena;

typedef struct
{
    memory_allocator_tag tag;
} memory_allocator_malloc;

typedef struct
{
    memory_allocator_tag tag;
} memory_allocator_os;

static memory_allocator_malloc global_memory_allocator_malloc;
static memory_allocator_os global_memory_allocator_os;

memory_allocator memory_allocator_arena_create(void *memory, uint64 size, char const *allocator_name)
{
    ASSERT_MSG(size >= 4096,
        "Cannot create memory arena, because given buffer size is less than 4k memory page. Given buffer is %llu bytes.", size);
    ASSERT_MSG(((uint64) memory) % alignof(memory_allocator_arena) == 0,
        "Cannot create memory arena, because alignment of given buffer is not %llu bytes.", (uint64) alignof(memory_allocator_arena));
    memory_allocator_arena *arena = (memory_allocator_arena *) memory;
    arena->tag = MemoryAllocator_Arena;
    arena->size = size - sizeof(memory_allocator_arena);
    arena->used = sizeof(memory_allocator_arena);
    arena->name = allocator_name;
    return arena;
}

memory_allocator memory_allocator_malloc_create(void)
{
    memory_allocator_malloc *result = &global_memory_allocator_malloc;
    result->tag = MemoryAllocator_Malloc;
    return result;
}

memory_allocator memory_allocator_os_create(void)
{
    memory_allocator_os *result = &global_memory_allocator_os;
    result->tag = MemoryAllocator_OS;
    return result;
}

void *memory_allocator_allocate_(memory_allocator a, uint64 size, uint64 alignment, code_location cl)
{
    void *result = NULL;

    memory_allocator_tag tag = *(memory_allocator_tag *) a;
    if (tag == MemoryAllocator_Arena)
    {
        memory_allocator_arena *arena = (memory_allocator_arena *) a;
        byte *base = ((byte *) arena) + arena->used;
        uint64 padding = get_padding(base, alignment);

        if (arena->used + size + padding <= arena->size)
        {
            arena->used += (size + padding);
            result = base + padding;
        }
#ifdef MEMORY_DEBUG_ENABLED
        printf("Allocator '%s': allocated %llu bytes from '%s()' (%s:%u)\n",
            arena->name, size, cl.function, cl.filename, cl.line);
#endif
    }
    else if (tag == MemoryAllocator_Malloc)
    {
        result = malloc(size);
#ifdef MEMORY_DEBUG_ENABLED
        printf("Allocator 'malloc': allocated %llu bytes from '%s()' (%s:%u)\n",
            size, cl.function, cl.filename, cl.line);
#endif
    }
    else
    {
        ASSERT_MSG(false, "Failed memory_allocator_allocate_ for MemoryAllocator_Invalid");
    }

    return result;
}

void *memory_allocator_allocate(memory_allocator a, uint64 size, uint64 alignment, code_location cl)
{
    void *result = memory_allocator_allocate_(a, size, alignment, cl);
    memset(result, 0, size);
    return result;
}

void memory_allocator_deallocate(memory_allocator a, void *memory, code_location cl)
{
    memory_allocator_tag tag = *(memory_allocator_tag *) a;
    if (tag == MemoryAllocator_Malloc)
    {
        free(memory);
    }
}

void memory_allocator_arena_reset(memory_allocator a)
{
    memory_allocator_tag tag = *(memory_allocator_tag *) a;
    ASSERT_IF(tag == MemoryAllocator_Arena)
    {
        memory_allocator_arena *arena = (memory_allocator_arena *) a;
        arena->used = sizeof(*arena);
    }
}

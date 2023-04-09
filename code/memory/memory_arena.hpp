#ifndef MEMORY_MEMORY_ARENA_HPP
#define MEMORY_MEMORY_ARENA_HPP

#include <base.hpp>
#include "allocator.hpp"


namespace memory {


struct memory_arena
{
    ALLOCATOR_BASE;
};

static_assert(sizeof(memory_arena) <= sizeof(allocator), "size of memory_arena is too large!");


void initialize_memory_arena(allocator *a, void *memory, usize size)
{
    auto *arena = (memory_arena *) a;
    arena->type = allocator::arena;
    arena->memory = memory;
    arena->size = size;
    arena->used = 0;
}

void initialize_memory_arena(allocator *a, memory_block block)
{
    initialize_memory_arena(a, block.memory, block.size);
}

void *arena_allocate(allocator *a, usize size, usize alignment)
{
    auto *arena = (memory_arena *) a;

    void *result = NULL;
    void *pointer = (byte *) arena->memory + arena->used;
    usize padding = get_padding(pointer, alignment);
    
    ASSERT((arena->used + padding + size) <= arena->size);
    if ((arena->used + padding + size) <= arena->size)
    {
        result = (byte *) arena->memory + arena->used + padding;
        arena->used += size + padding;
    }

    return result;
}

void arena_reset(allocator *a)
{
    a->used = 0;
}

} // namespace memory

#endif // MEMORY_MEMORY_ARENA_HPP

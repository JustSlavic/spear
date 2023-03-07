#include "allocator.hpp"

#include <memory/memory_arena.cpp>
// #include <memory/memory_stack.cpp>
// #include <memory/memory_pool.cpp>
// #include <memory/memory_heap.cpp>


namespace memory {

// void initialize_memory_stack(allocator *a, void *memory, usize size);
// void initialize_memory_pool(allocator *a, void *memory, usize size);
// void initialize_memory_heap(allocator *a, void *memory, usize size);

void *allocate_(allocator *a, usize size, usize alignment)
{
    void *result = NULL;
    switch (a->type)
    {
        case allocator::arena: result = arena_allocate(a, size, alignment);
            break;
        case allocator::stack: // @todo
            break;
        case allocator::pool: // @todo
            break;
        case allocator::heap: // @todo
            break;
    }
    return result;
}

void *allocate(allocator *a, usize size, usize alignment)
{
    void *result = allocate_(a, size, alignment);
    if (result)
    {
        memory::set(result, 0, size);
    }
    return result;
}

// @todo
// array allocate_array(allocator *a, usize size, usize alignment);

memory_block allocate_block_(allocator *a, usize size, usize alignment)
{
    memory_block result;
    result.memory = allocate_(a, size, alignment);
    result.size   = size;
    return result;
}

memory_block allocate_block(allocator *a, usize size, usize alignment)
{
    memory_block result;
    result.memory = allocate(a, size, alignment);
    result.size   = size;
    return result;
}

// @todo
// void *reallocate(allocator *a, void *memory, usize size);

// @todo
// void deallocate(allocator *a, void *memory, usize size);

void reset_allocator(allocator *a)
{
    switch (a->type)
    {
        case allocator::arena: arena_reset(a);
            break;
        case allocator::stack: // @todo
            break;
        case allocator::pool: // @todo
            break;
        case allocator::heap: // @todo
            break;
    }
}


} // namespace memory

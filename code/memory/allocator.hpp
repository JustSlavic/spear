#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include <base.hpp>
#include <memory/memory.hpp>


#define ALLOCATOR_BASE struct { ::memory::allocator::allocator_type type; void *memory; usize size; usize used; }

#define ALLOCATE_(ALLOCATOR, TYPE) (TYPE *) allocate_(ALLOCATOR, sizeof(TYPE), alignof(TYPE))
#define ALLOCATE(ALLOCATOR, TYPE) (TYPE *) allocate(ALLOCATOR, sizeof(TYPE), alignof(TYPE))

#define ALLOCATE_BUFFER_(ALLOCATOR, SIZE) (void *) allocate_(ALLOCATOR, SIZE, 1);
#define ALLOCATE_BUFFER(ALLOCATOR, SIZE) (void *) allocate(ALLOCATOR, SIZE, 1);

// #define ALLOCATE_ARRAY(ALLOCATOR, TYPE, COUNT) allocate_array(ALLOCATOR, sizeof(TYPE) * COUNT, alignof(TYPE))

#define ALLOCATE_BLOCK_(ALLOCATOR, SIZE) allocate_block_(ALLOCATOR, SIZE, 1);
#define ALLOCATE_BLOCK(ALLOCATOR, SIZE) allocate_block(ALLOCATOR, SIZE, 1);


namespace memory {

struct allocator
{
    enum allocator_type
    {
        arena,
        stack,
        pool,
        heap,
    };

    ALLOCATOR_BASE;
    uint8_t data[16];
};


void initialize_memory_arena(allocator *a, void *memory, usize size);
// void initialize_memory_stack(allocator *a, void *memory, usize size);
// void initialize_memory_pool(allocator *a, void *memory, usize size);
// void initialize_memory_heap(allocator *a, void *memory, usize size);

void *allocate_(allocator *a, usize size, usize alignment);
void *allocate(allocator *a, usize size, usize alignment);
// array allocate_array(allocator *a, usize size, usize alignment);
memory_block allocate_block_(allocator *a, usize size, usize alignment);
memory_block allocate_block(allocator *a, usize size, usize alignment);
// void *reallocate(allocator *a, void *memory, usize size);
// void deallocate(allocator *a, void *memory, usize size);

void reset_allocator(allocator *a);

} // namespace memory


#endif // MEMORY_ALLOCATOR_HPP

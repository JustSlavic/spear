#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include <base.hpp>
#include <memory/memory.hpp>


#define ALLOCATOR_BASE struct \
{ \
    ::memory::allocator::allocator_type type; \
    void *memory; \
    usize size;   \
    usize used;   \
}

#define ALLOCATE_(ALLOCATOR, TYPE) (TYPE *) allocate_(ALLOCATOR, sizeof(TYPE), alignof(TYPE))
#define ALLOCATE(ALLOCATOR, TYPE) (TYPE *) allocate(ALLOCATOR, sizeof(TYPE), alignof(TYPE))
#define DEALLOCATE(ALLOCATOR, POINTER) deallocate(ALLOCATOR, POINTER)

// #define REALLOCATE

#define ALLOCATE_BUFFER_(ALLOCATOR, SIZE) (void *) allocate_(ALLOCATOR, SIZE, 1)
#define ALLOCATE_BUFFER(ALLOCATOR, SIZE) (void *)  allocate (ALLOCATOR, SIZE, 1)

#define ALLOCATE_BUFFER_ALIGNED_(ALLOCATOR, SIZE, ALIGNMENT) (void *) allocate_(ALLOCATOR, SIZE, ALIGNMENT)
#define ALLOCATE_BUFFER_ALIGNED(ALLOCATOR, SIZE, ALIGNMENT)  (void *) allocate (ALLOCATOR, SIZE, ALIGNMENT)

#define ALLOCATE_BUFFER_TYPED_(ALLOCATOR, TYPE, COUNT) (TYPE *) allocate_(ALLOCATOR, sizeof(TYPE) * COUNT, alignof(TYPE))
#define ALLOCATE_BUFFER_TYPED(ALLOCATOR, TYPE, COUNT)  (TYPE *) allocate (ALLOCATOR, sizeof(TYPE) * COUNT, alignof(TYPE))

// #define ALLOCATE_ARRAY(ALLOCATOR, TYPE, COUNT) allocate_array(ALLOCATOR, sizeof(TYPE) * COUNT, alignof(TYPE))

#define ALLOCATE_BLOCK_(ALLOCATOR, SIZE) allocate_block_(ALLOCATOR, SIZE, 1)
#define ALLOCATE_BLOCK(ALLOCATOR, SIZE)  allocate_block (ALLOCATOR, SIZE, 1)

#define ALLOCATE_COPY(ALLOCATOR, OBJECT) allocate_copy(ALLOCATOR, OBJECT)
#define ALLOCATE_COPY_TO_BLOCK(ALLOCATOR, OBJECT) allocate_copy_to_block(ALLOCATOR, OBJECT)

// ======================== TEMPORARY ALLOCATIONS ======================== //

#define T_ALLOCATE_(TYPE) (TYPE *) temporary_allocate_(sizeof(TYPE), alignof(TYPE))
#define T_ALLOCATE(TYPE)  (TYPE *) temporary_allocate (sizeof(TYPE), alignof(TYPE))

#define T_ALLOCATE_BUFFER_(SIZE) (void *) temporary_allocate_(SIZE, 1)
#define T_ALLOCATE_BUFFER(SIZE)  (void *) temporary_allocate (SIZE, 1)

#define T_ALLOCATE_BUFFER_ALIGNED_(SIZE, ALIGNMENT) (void *) temporary_allocate_(SIZE, ALIGNMENT)
#define T_ALLOCATE_BUFFER_ALIGNED(SIZE, ALIGNMENT)  (void *) temporary_allocate (SIZE, ALIGNMENT)

#define T_ALLOCATE_BUFFER_OF_(TYPE, COUNT) (TYPE *) temporary_allocate_(sizeof(TYPE) * COUNT, alignof(TYPE))
#define T_ALLOCATE_BUFFER_OF(TYPE, COUNT)  (TYPE *) temporary_allocate (sizeof(TYPE) * COUNT, alignof(TYPE))

#define T_ALLOCATE_BLOCK_(SIZE) temporary_allocate_block_(SIZE, 1)
#define T_ALLOCATE_BLOCK(SIZE)  temporary_allocate_block (SIZE, 1)

#define T_ALLOCATE_COPY(OBJECT) temporary_allocate_copy(OBJECT)
#define T_ALLOCATE_COPY_TO_BLOCK(OBJECT) temporary_allocate_copy_to_block(OBJECT)


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
    uint8 data[16];
};


void initialize_memory_arena(allocator *a, void *memory, usize size);
void initialize_memory_arena(allocator *a, memory_block block);
// void initialize_memory_stack(allocator *a, void *memory, usize size);
// void initialize_memory_pool(allocator *a, void *memory, usize size);
void initialize_memory_heap(allocator *a, void *memory, usize size);
void initialize_memory_heap(allocator *a, memory_block block);
void reset_allocator(allocator *a);

void *allocate_(allocator *a, usize size, usize alignment);
void *allocate(allocator *a, usize size, usize alignment);
void deallocate(allocator *a, void *memory);
// void *reallocate(allocator *a, void *memory, usize size);
memory_block allocate_block_(allocator *a, usize size, usize alignment);
memory_block allocate_block(allocator *a, usize size, usize alignment);
// array allocate_array(allocator *a, usize size, usize alignment);

template <typename T>
T *allocate_copy(allocator *a, T *pointer)
{
    auto *result = ALLOCATE_(a, T);
    memory::copy(result, pointer, sizeof(T));
    return result;
}

memory_block allocate_copy(allocator *a, memory_block block)
{
    memory_block result = ALLOCATE_BLOCK(a, block.size);
    memory::copy(result.memory, block.memory, block.size);
    return result;
}

template <typename T>
memory_block allocate_copy_to_block(allocator *a, T *pointer)
{
    T *result_pointer = ALLOCATE_(a, T);
    memory_block result = { result_pointer, sizeof(T) };
    memory::copy(result.memory, pointer, sizeof(T));
    return result;
}

memory_block allocate_copy_to_block(allocator *a, memory_block block)
{
    memory_block result = ALLOCATE_BLOCK(a, block.size);
    memory::copy(result.memory, block.memory, block.size);
    return result;
}

// void initialize_temporary_arena(void *memory, usize size);
// void initialize_temporary_arena(memory_block block);
// void *temporary_allocate_(usize size, usize alignment);
// void *temporary_allocate(usize size, usize alignment);
// memory_block temporary_allocate_block_(usize size, usize alignment);
// memory_block temporary_allocate_block(usize size, usize alignment);


} // namespace memory


#endif // MEMORY_ALLOCATOR_HPP

#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include <base.hpp>


struct allocator
{
    void *memory;
    usize size;

    uint8_t data[16];
};


#endif // MEMORY_ALLOCATOR_HPP

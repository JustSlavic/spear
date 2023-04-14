#ifndef C_ARRAY_HPP
#define C_ARRAY_HPP

#include <base.hpp>


typedef struct array_header_t
{
    usize size;
    usize capacity;
} array_header_t;


#define c_array_allocate(ALLOCATOR, TYPE, SIZE)

#define c_array_size(A) ((((array_header_t *)(A)) - 1)->size)
#define c_array_capacity(A) ((((array_header_t *)(A)) - 1)->capacity)


#endif // C_ARRAY_HPP

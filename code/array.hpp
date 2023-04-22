#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <base.hpp>


template <typename Type>
struct array
{
    Type *data;
    usize size;
    usize capacity;

    Type& operator[] (usize index)
    {
        return data[index];
    }

    Type *push_back(Type t)
    {
        Type* result = 0;
        if (size < capacity)
        {
            usize index = size++;
            result = data + index;
            data[index] = t;
        }
        return result;
    }
};

#define ALLOCATE_ARRAY_(ALLOCATOR, TYPE, CAPACITY) array<TYPE>{ALLOCATE_BUFFER_TYPED_(ALLOCATOR, TYPE, CAPACITY), 0, (CAPACITY)}
#define ALLOCATE_ARRAY(ALLOCATOR, TYPE, CAPACITY) array<TYPE>{ALLOCATE_BUFFER_TYPED(ALLOCATOR, TYPE, CAPACITY), 0, (CAPACITY)}


template <typename Type, usize Capacity>
struct static_array
{
    Type data[Capacity];
    usize size;
};


template <typename Type, usize Capacity>
Type *push_back(static_array<Type, Capacity> *a, Type t)
{
    Type *result = NULL;
    if (a->size < Capacity)
    {
        usize index = a->size++;
        result = a->data + index;
        a->data[index] = t;
    }
}


#endif // ARRAY_HPP

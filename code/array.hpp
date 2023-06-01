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

    Type const& operator[] (usize index) const
    {
        return data[index];
    }

    Type *push()
    {
        Type* result = 0;
        if (size < capacity)
        {
            usize index = size++;
            result = data + index;
        }
        return result;
    }

    void push(Type t)
    {
        if (size < capacity)
        {
            data[size++] = t;
        }
    }
};

#define ALLOCATE_ARRAY_(ALLOCATOR, TYPE, CAPACITY) array<TYPE>{ALLOCATE_BUFFER_TYPED_(ALLOCATOR, TYPE, CAPACITY), 0, (CAPACITY)}
#define ALLOCATE_ARRAY(ALLOCATOR, TYPE, CAPACITY) array<TYPE>{ALLOCATE_BUFFER_TYPED(ALLOCATOR, TYPE, CAPACITY), 0, (CAPACITY)}


template <typename Type, usize Capacity>
struct static_array
{
    Type data[Capacity];
    usize size;

    usize capacity() { return Capacity; }

    Type& operator[] (usize index)
    {
        return data[index];
    }

    Type const& operator[] (usize index) const
    {
        return data[index];
    }

    Type *push()
    {
        Type* result = 0;
        if (size < Capacity)
        {
            usize index = size++;
            result = data + index;
        }
        return result;
    }

    void push(Type t)
    {
        if (size < Capacity)
        {
            usize index = size++;
            data[index] = t;
        }
    }

    void erase_not_sorted(usize index)
    {
        if (index < size)
        {
            data[index] = data[size - 1];
            size -= 1;
        }
    }
};


#endif // ARRAY_HPP

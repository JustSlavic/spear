#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <base.h>
#include <memory.h>


template <typename Type>
struct array
{
    Type *data_;
    usize size_;
    usize capacity_;

    Type *data() { return data_; }
    usize size() { return size_; }
    usize capacity() { return capacity_; }
    void clear() { size_ = 0; }
    bool empty() { return (size_ == 0); }

    Type& operator[] (usize index)
    {
        ASSERT(index < size_);
        return data_[index];
    }

    Type const& operator[] (usize index) const
    {
        ASSERT(index < size_);
        return data_[index];
    }

    Type *push()
    {
        Type* result = 0;
        if (size_ < capacity_)
        {
            usize index = size_++;
            result = data_ + index;
        }
        return result;
    }

    void push(Type t)
    {
        if (size_ < capacity_)
        {
            data_[size_++] = t;
        }
    }
};


template <typename Type>
array<Type> create_array_from_block(memory_block block)
{
    array<Type> result;
    result.data_ = (Type *) block.memory;
    result.size_ = 0;
    result.capacity_ = (block.size / sizeof(Type));

    return result;
}

template <typename Type>
array<Type> create_array_from_block_open(memory_block block)
{
    auto result = create_array_from_block<Type>(block);
    result.size_ = result.capacity_;
    return result;
}

#define ALLOCATE_ARRAY_(ALLOCATOR, TYPE, CAPACITY) create_array_from_block<TYPE>(ALLOCATE_BUFFER_ALIGNED_(ALLOCATOR, (CAPACITY) * sizeof(TYPE), alignof(TYPE)))
#define ALLOCATE_ARRAY(ALLOCATOR, TYPE, CAPACITY) create_array_from_block<TYPE>(ALLOCATE_BUFFER_ALIGNED(ALLOCATOR, (CAPACITY) * sizeof(TYPE), alignof(TYPE)))

#define ALLOCATE_ARRAY_OPEN(ALLOCATOR, TYPE, CAPACITY) create_array_from_block_open<TYPE>(ALLOCATE_BUFFER_ALIGNED(ALLOCATOR, (CAPACITY) * sizeof(TYPE), alignof(TYPE)))


template <typename Type, usize Capacity>
struct static_array
{
    Type data_[Capacity];
    usize size_;

    Type *data() { return data_; }
    usize size() { return size_; }
    usize capacity() { return Capacity; }
    void clear() { size_ = 0; }
    bool empty() { return (size_ == 0); }

    Type& operator[] (usize index)
    {
        return data_[index];
    }

    Type const& operator[] (usize index) const
    {
        return data_[index];
    }

    Type *push()
    {
        Type* result = 0;
        if (size_ < Capacity)
        {
            usize index = size_++;
            result = data_ + index;
        }
        return result;
    }

    void push(Type t)
    {
        if (size_ < Capacity)
        {
            usize index = size_++;
            data_[index] = t;
        }
    }

    void erase_not_sorted(usize index)
    {
        if (index < size_)
        {
            data_[index] = data_[size_ - 1];
            size_ -= 1;
        }
    }
};


#endif // ARRAY_HPP

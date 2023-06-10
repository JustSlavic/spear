#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <base.hpp>


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
        return data_[index];
    }

    Type const& operator[] (usize index) const
    {
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

#define ALLOCATE_ARRAY_(ALLOCATOR, TYPE, CAPACITY) array<TYPE>{ALLOCATE_BUFFER_TYPED_(ALLOCATOR, TYPE, CAPACITY), 0, (CAPACITY)}
#define ALLOCATE_ARRAY(ALLOCATOR, TYPE, CAPACITY) array<TYPE>{ALLOCATE_BUFFER_TYPED(ALLOCATOR, TYPE, CAPACITY), 0, (CAPACITY)}


template <typename Type, usize Capacity>
struct static_array
{
    Type data_[Capacity];
    usize size_;

    Type *data() { return data_; }
    usize size() { return size_; }
    usize capacity() { return Capacity; }
    void reset() { size_ = 0; }
    bool empty() { return (size_ == 0); }

    Type& operator[] (usize index)
    {
        return data_[index];
    }

    Type const& operator[] (usize index) const
    {
        return data[index];
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

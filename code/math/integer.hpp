#ifndef MATH_INTEGER_HPP
#define MATH_INTEGER_HPP

#include <base.hpp>


INLINE int32 int64_to_int32(int64 x)
{
    int32 result = (int32) x;
    return result;
}

INLINE uint32 uint64_to_uint32(uint64 x)
{
    uint32 result = (uint32) x;
    return result;
}

INLINE uint32 usize_to_uint32(usize x)
{
    uint32 result = (uint32) x;
    return result;
}

INLINE int32 usize_to_int32(usize x)
{
    int32 result = (int32) x;
    return result;
}


INLINE int32 min_(int32 x, int32 y)
{
    int32 result = (x < y) ? x : y;
    return result;
}

INLINE int32 max_(int32 x, int32 y)
{
    int32 result = (x < y) ? y : x;
    return result;
}


#endif // MATH_INTEGER_HPP

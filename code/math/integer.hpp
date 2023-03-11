#ifndef MATH_INTEGER_HPP
#define MATH_INTEGER_HPP

#include <base.hpp>


INLINE int32 truncate_int32(int64 x)
{
    int32 result = (int32) x;
    return result;
}

INLINE uint32 truncate_uint32(uint64 x)
{
    uint32 result = (uint32) x;
    return result;
}


#endif // MATH_INTEGER_HPP

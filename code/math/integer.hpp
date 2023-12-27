#ifndef MATH_INTEGER_HPP
#define MATH_INTEGER_HPP

#include <base.h>


INLINE int32 truncate_to_int32(int64 x)
{
    int32 result = (int32) x;
    return result;
}

INLINE uint32 truncate_to_uint32(uint64 x)
{
    uint32 result = (uint32) x;
    return result;
}

INLINE int32 truncate_to_int32(usize x)
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


namespace math {

int32 absolute(int32 x)
{
    int32 result = x < 0 ? -x : x;
    return result;
}

int64 absolute(int64 x)
{
    int64 result = x < 0 ? -x : x;
    return result;
}

}


#endif // MATH_INTEGER_HPP

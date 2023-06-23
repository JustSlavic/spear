#ifndef MATH_INTEGER_HPP
#define MATH_INTEGER_HPP

#include <base.hpp>


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

int8 change_endianness(int8 n) {
    return n;
}

uint8 change_endianness(uint8 n) {
    return n;
}

int16 change_endianness(int16 n) {
    int16 result = ((n & 0xFF00) >> 8) |
                   ((n & 0x00FF) << 8);
    return result;
}

uint16 change_endianness(uint16 n) {
    uint16 result = ((n & 0xFF00) >> 8) |
                    ((n & 0x00FF) << 8);
    return result;
}

int32 change_endianness(int32 n) {
    int32 result = ((n & 0xFF000000) >> 24) |
                   ((n & 0x00FF0000) >>  8) |
                   ((n & 0x0000FF00) <<  8) |
                   ((n & 0x000000FF) << 24);
    return result;
}

uint32 change_endianness(uint32 n) {
    uint32 result = ((n & 0xFF000000) >> 24) |
                    ((n & 0x00FF0000) >>  8) |
                    ((n & 0x0000FF00) <<  8) |
                    ((n & 0x000000FF) << 24);
    return result;
}

int64 change_endianness(int64 n) {
    int64 result = ((n & 0xFF00000000000000) >> 56) |
                   ((n & 0x00FF000000000000) >> 40) |
                   ((n & 0x0000FF0000000000) >> 24) |
                   ((n & 0x000000FF00000000) >>  8) |
                   ((n & 0x00000000FF000000) <<  8) |
                   ((n & 0x0000000000FF0000) << 24) |
                   ((n & 0x000000000000FF00) << 40) |
                   ((n & 0x00000000000000FF) << 56);
    return result;
}

uint64 change_endianness(uint64 n) {
    uint64 result = ((n & 0xFF00000000000000) >> 56) |
                    ((n & 0x00FF000000000000) >> 40) |
                    ((n & 0x0000FF0000000000) >> 24) |
                    ((n & 0x000000FF00000000) >>  8) |
                    ((n & 0x00000000FF000000) <<  8) |
                    ((n & 0x0000000000FF0000) << 24) |
                    ((n & 0x000000000000FF00) << 40) |
                    ((n & 0x00000000000000FF) << 56);
    return result;
}


#endif // MATH_INTEGER_HPP

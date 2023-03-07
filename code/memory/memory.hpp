#ifndef MEMORY_MEMORY_HPP
#define MEMORY_MEMORY_HPP

#include <base.hpp>


namespace memory
{


void *set(void *destination, int value, usize count)
{
    byte *d = (byte *) destination;
    while (count--)
    {
        *d++ = (byte) value;
    }
    return destination;
}


void *copy(void *destination, void const *source, usize count)
{
    byte *d = (byte *) destination;
    byte *s = (byte *) source;
    while (count--)
    {
        *d++ = *s++;
    }
    return destination;
}


usize get_padding(void *pointer, usize alignment)
{
    usize result = (alignment - ((usize)pointer & (alignment - 1))) & (alignment - 1);
    return result;
}


void *align_pointer(void *pointer, usize alignment)
{
    void *result = (byte *) pointer + get_padding(pointer, alignment);
    return result;
}


} // namespace memory


struct memory_block
{
    void *memory;
    usize size;
};


#endif // MEMORY_MEMORY_HPP

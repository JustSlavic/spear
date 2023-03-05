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


} // namespace memory


struct memory_block
{
    void *memory;
    usize size;
};


#endif // MEMORY_MEMORY_HPP

#ifndef PLATFORM_SDL_HPP
#define PLATFORM_SDL_HPP

// My library
#include <base.h>
#include <memory/memory.hpp>
#include <memory/buffer.hpp>
// Standard library
#include <stdlib.h>


namespace sdl {


INLINE memory_buffer allocate_memory(void *base_address, usize size)
{
    memory_buffer result;
    result.data = (byte *) malloc(size);
    result.size = size;
    memset(result.data, 0, size);

    return result;
}


}


#endif // PLATFORM_SDL_HPP

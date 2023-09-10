#ifndef PLATFORM_SDL_HPP
#define PLATFORM_SDL_HPP

// My library
#include <base.h>
#include <memory/memory.hpp>
// Standard library
#include <stdlib.h>


namespace sdl {


INLINE memory_block allocate_memory(void *base_address, usize size)
{
    memory_block result;
    result.memory = malloc(size);
    result.size   = size;
    memory::set(result.memory, 0, size);

    return result;
}


}


#endif // PLATFORM_SDL_HPP

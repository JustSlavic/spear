#ifndef GFX__GL_MAC_HPP
#define GFX__GL_MAC_HPP

#include <base.h>


namespace gl {


bool32 initialize()
{
    return true;
}

void vsync(bool turn_on)
{
}

void swap_buffers(void *w)
{
}


} // namespace gl


#endif // GFX__GL_MAC_HPP

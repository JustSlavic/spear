#ifndef GFX_GL_MAC_HPP
#define GFX_GL_MAC_HPP

#include <base.hpp>


namespace gfx {
namespace gl {


bool32 initialize()
{
    return true;
}

void vsync(void *window, bool turn_on)
{
}

void swap_buffers(void *window)
{
}

void destroy_window_and_driver(void *window, void *driver)
{
}


} // namespace gfx
} // namespace gl


#endif // GFX_GL_MAC_HPP

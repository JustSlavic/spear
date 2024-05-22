#ifndef GFX__GL_MAC_HPP
#define GFX__GL_MAC_HPP

#include <base.h>


#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE


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
    auto window = (sdl::window *) w;
    SDL_GL_SwapWindow(window->handle);
}

void use_texture(uint32 texture_id, uint32 slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    GL_CHECK_ERRORS();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    GL_CHECK_ERRORS();
}

} // namespace gl


#endif // GFX__GL_MAC_HPP

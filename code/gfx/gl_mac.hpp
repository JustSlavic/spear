#ifndef GFX__GL_MAC_HPP
#define GFX__GL_MAC_HPP

#include <base.h>
#include <dlfcn.h>


namespace gl {


bool32 initialize()
{
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("Shading language supported: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void vsync(bool turn_on)
{
    int ec = SDL_GL_SetSwapInterval(turn_on ? 1 : 0);
    if (ec < 0)
    {
        printf("SDL_GL_SetSwapInterval not supported: \"%s\"", SDL_GetError());
    }
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

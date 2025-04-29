#ifndef PLATFORM_SDL_HPP
#define PLATFORM_SDL_HPP

// My library
#include <base.h>
#include <memory/memory.hpp>
#include <memory/buffer.hpp>
// Standard library
#include <stdlib.h>
#include <sys/mman.h>
// SDL2
#include <SDL2/SDL.h>


namespace sdl {


INLINE memory_buffer allocate_memory(void *base_address, usize size)
{
    memory_buffer result;
    result.data = (byte *) mmap(base_address, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    result.size = size;
    memset(result.data, 0, size);

    return result;
}


struct window
{
    SDL_Window *handle;
    SDL_GLContext context;
};


void create_opengl_window(int32 width, int32 height, window *w)
{
    int32 err = SDL_Init(SDL_INIT_VIDEO);
    if (err < 0)
    {
        printf("Could not initialize SDL2: \"%s\"\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    auto window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        printf("Could not create SDL Window: \"%s\"\n", SDL_GetError());
        exit(2);
    }

    // @warning: SDL_GetWindowSurface resets flags within window!
    // https://discourse.libsdl.org/t/sdl-window-opengl-on-mac-sonoma-sdl-2-28-5-not-working/48866

    // auto surface = SDL_GetWindowSurface(window);
    // if (surface == NULL)
    // {
    //     printf("Could not create SDL window surface: \"%s\"\n", SDL_GetError());
    //     exit(3);
    // }

    auto context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        printf("Could not create OpenGL context: \"%s\"\n", SDL_GetError());
        exit(4);
    }

    w->handle  = window;
    w->context = context;
        
    SDL_version compiled;
    SDL_VERSION(&compiled);
    printf("Compiled against SDL2 v.%d.%d.%d\n", (int)compiled.major, (int)compiled.minor, (int)compiled.patch);

    SDL_version linked;
    SDL_GetVersion(&linked);
    printf("Linked against SDL2 v.%d.%d.%d\n", (int)linked.major, (int)linked.minor, (int)linked.patch);
}


void get_mouse_pos(int *x, int *y)
{
    SDL_GetMouseState(x, y);
}

}


#endif // PLATFORM_SDL_HPP

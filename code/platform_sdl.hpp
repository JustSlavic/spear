#ifndef PLATFORM_SDL_HPP
#define PLATFORM_SDL_HPP

// My library
#include <base.h>
#include <memory/memory.hpp>
#include <memory/buffer.hpp>
// Standard library
#include <stdlib.h>
// SDL2
#include <SDL2/SDL.h>


namespace sdl {


INLINE memory_buffer allocate_memory(void *base_address, usize size)
{
    memory_buffer result;
    result.data = (byte *) malloc(size);
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

keyboard_key map_button_from_scancode(uint32 vk)
{
    switch (vk)
    {
        case SDL_SCANCODE_ESCAPE: return KB_ESC; break;

        case SDL_SCANCODE_A: return KB_A; break;
        case SDL_SCANCODE_B: return KB_B; break;
        case SDL_SCANCODE_C: return KB_C; break;
        case SDL_SCANCODE_D: return KB_D; break;
        case SDL_SCANCODE_E: return KB_E; break;
        case SDL_SCANCODE_F: return KB_F; break;
        case SDL_SCANCODE_G: return KB_G; break;
        case SDL_SCANCODE_H: return KB_H; break;
        case SDL_SCANCODE_I: return KB_I; break;
        case SDL_SCANCODE_J: return KB_J; break;
        case SDL_SCANCODE_K: return KB_K; break;
        case SDL_SCANCODE_L: return KB_L; break;
        case SDL_SCANCODE_M: return KB_M; break;
        case SDL_SCANCODE_N: return KB_N; break;
        case SDL_SCANCODE_O: return KB_O; break;
        case SDL_SCANCODE_P: return KB_P; break;
        case SDL_SCANCODE_Q: return KB_Q; break;
        case SDL_SCANCODE_R: return KB_R; break;
        case SDL_SCANCODE_S: return KB_S; break;
        case SDL_SCANCODE_T: return KB_T; break;
        case SDL_SCANCODE_U: return KB_U; break;
        case SDL_SCANCODE_V: return KB_V; break;
        case SDL_SCANCODE_W: return KB_W; break;
        case SDL_SCANCODE_X: return KB_X; break;
        case SDL_SCANCODE_Y: return KB_Y; break;
        case SDL_SCANCODE_Z: return KB_Z; break;

        case SDL_SCANCODE_F1: return KB_F1; break;
        case SDL_SCANCODE_F2: return KB_F2; break;
        case SDL_SCANCODE_F3: return KB_F3; break;
        case SDL_SCANCODE_F4: return KB_F4; break;
        case SDL_SCANCODE_F5: return KB_F5; break;
        case SDL_SCANCODE_F6: return KB_F6; break;
        case SDL_SCANCODE_F7: return KB_F7; break;
        case SDL_SCANCODE_F8: return KB_F8; break;
        case SDL_SCANCODE_F9: return KB_F9; break;
        case SDL_SCANCODE_F10: return KB_F10; break;
        case SDL_SCANCODE_F11: return KB_F11; break;
        case SDL_SCANCODE_F12: return KB_F12; break;

        case SDL_SCANCODE_SPACE: return KB_SPACE; break;

        case SDL_SCANCODE_LCTRL: return KB_CTRL; break;
        case SDL_SCANCODE_LSHIFT: return KB_SHIFT; break;
        case SDL_SCANCODE_LALT: return KB_ALT; break;
    }
    return KB_NONE;
}


}


#endif // PLATFORM_SDL_HPP

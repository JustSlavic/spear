#include <corelibs/base.h>
#include <corelibs/platform.h>
#include <corelibs/memory/allocator.h>
#include <gamelibs/input.h>
#include <engine/engine.h>
#include <engine/game_interface.h>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static spear_engine g_engine;
static spear_input g_input;
static bool32 g_window_size_changed;

uint32 scancode_to_button_id[] =
{
#include "sdl_scancode_to_button.inl"
};


void process_pending_messages(spear_input *input)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type)
        {
            case SDL_QUIT:
                g_engine.running = false;
            break;

            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
                    e.window.event == SDL_WINDOWEVENT_SHOWN ||
                    e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    g_window_size_changed = true;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                int32 button = (e.button.button == 1) ? Mouse_Left :
                               (e.button.button == 2) ? Mouse_Middle :
                               (e.button.button == 3) ? Mouse_Right :
                                                        Button_None;
                input_button_update(
                    &input->keyboard_and_mouse.buttons[button],
                    e.button.state == SDL_PRESSED);
            }
            break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                ASSERT(e.key.keysym.scancode < ARRAY_COUNT(scancode_to_button_id));
                uint32 button = scancode_to_button_id[e.key.keysym.scancode];
                input_button_update(
                    &input->keyboard_and_mouse.buttons[button],
                    e.type == SDL_KEYDOWN);
            }
            break;
        }
    }
}

typedef struct
{
    uint32 channel_count;
    uint32 samples_per_second;
    uint32 sample_size;
    uint32 sound_buffer_size;
    uint32 samples_filled_count;
    int16 *samples;

    bool32 is_playing;
} sound_output_t;

void sdl_audio_callback(void *userdata, uint8 *buffer_to_write_to, int requested_length)
{
    if (userdata)
    {
        sound_output_t *sound_output = (sound_output_t *) userdata;
        uint32 bytes_filled = sound_output->samples_filled_count * sound_output->sample_size;
        uint32 samples_requested = requested_length / sound_output->sample_size;

        if (requested_length <= bytes_filled)
        {
            memcpy(buffer_to_write_to, sound_output->samples, requested_length);
            if (requested_length < bytes_filled)
                memcpy(sound_output->samples, (uint8 *) sound_output->samples + requested_length, bytes_filled - requested_length);
            sound_output->samples_filled_count -= samples_requested;
        }
        else
        {
            memcpy(buffer_to_write_to, sound_output->samples, bytes_filled);
            sound_output->samples_filled_count = 0;
        }
    }
}


int main(void)
{
    spear_engine_init(&g_engine);

    // ===================================================

    int err = SDL_Init(SDL_INIT_VIDEO);
    if (err < 0)
    {
        printf("Could not initialize SDL2: \"%s\"\n", SDL_GetError());
        exit(2);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        g_engine.current_client_width, g_engine.current_client_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
        printf("Could not create SDL Window: \"%s\"\n", SDL_GetError());
        exit(3);
    }

    SDL_GLContext *render_context = SDL_GL_CreateContext(window);
    if (render_context == NULL)
    {
        printf("Could not create OpenGL context: \"%s\"\n", SDL_GetError());
        exit(4);
    }

    SDL_GL_MakeCurrent(window, render_context);

    SDL_version compiled;
    SDL_VERSION(&compiled);
    printf("Compiled against SDL2 v.%d.%d.%d\n", (int)compiled.major, (int)compiled.minor, (int)compiled.patch);

    SDL_version linked;
    SDL_GetVersion(&linked);
    printf("Linked against SDL2 v.%d.%d.%d\n", (int)linked.major, (int)linked.minor, (int)linked.patch);

    printf("glGetString(GL_SHADING_LANGUAGE_VERSION) -> %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    GLubyte const *gl_version = glGetString(GL_VERSION);
    printf("glGetString(GL_VERSION) -> %s\n", gl_version);

    // ===================================================

    spear_engine_init_graphics(&g_engine);

    // ===================================================

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);

    // ===================================================

    spear_engine_create_meshes(&g_engine);
    spear_engine_load_game_data(&g_engine);
    spear_engine_compile_shaders(&g_engine);
    spear_engine_load_game_dll(&g_engine, "spear_game.so");
    spear_engine_game_init(&g_engine);

    // ===================================================

    sound_output_t sound_output = {};
    sound_output.channel_count = 2;
    sound_output.samples_per_second = 44100;
    sound_output.sample_size = sound_output.channel_count * sizeof(int16);
    sound_output.sound_buffer_size = sound_output.samples_per_second * sound_output.sample_size;
    sound_output.samples_filled_count = 0;
    sound_output.samples = ALLOCATE_BUFFER_(g_engine.allocator, sound_output.sound_buffer_size);

    sound_output.is_playing = false;

    {
        SDL_AudioSpec sdl_spec = {};
        sdl_spec.format = AUDIO_S16LSB;
        sdl_spec.freq = sound_output.samples_per_second;
        sdl_spec.channels = sound_output.channel_count;
        sdl_spec.samples = 0;
        sdl_spec.size = 0;
        sdl_spec.callback = sdl_audio_callback;
        sdl_spec.userdata = &sound_output;

        if (SDL_OpenAudio(&sdl_spec, NULL) < 0)
        {
            printf("Error: SDL could not open audio device\n");
            return 0;
        }
    }

    duration last_frame_dt = duration_create_milliseconds(16);
    timepoint last_timepoint = platform_clock_now();

    g_engine.running = true;
    while (g_engine.running)
    {
        memory_allocator_arena_reset(g_engine.temporary);
        spear_engine_load_game_dll(&g_engine, "spear_game.so");

        input_button_reset_transitions(g_input.keyboard_and_mouse.buttons, Keyboard_Button_Count);
        g_input.keyboard_and_mouse.mouse_scroll = 0;

        process_pending_messages(&g_input);
        // Set mouse pos
        {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            spear_engine_input_mouse_pos_set(&g_engine, &g_input, mouse_x, mouse_y);
        }

        g_input.dt = duration_get_seconds(last_frame_dt);
        g_input.time = timepoint_get_seconds(last_timepoint);

        if (g_window_size_changed)
        {
            g_window_size_changed = false;
            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            spear_engine_update_viewport(&g_engine, width, height);
        }
        spear_engine_game_update(&g_engine, &g_input);
        spear_engine_game_render(&g_engine);

        {
            uint32 samples_per_game_frame = (sound_output.samples_per_second / 60);
            uint32 latency_frames = (4 * samples_per_game_frame);
            uint32 samples_to_fill = 0;
            if (sound_output.samples_filled_count < latency_frames)
            {
                samples_to_fill = (latency_frames - sound_output.samples_filled_count);
            }

            spear_sound_output_buffer sound_from_game = {};
            sound_from_game.samples = sound_output.samples;
            sound_from_game.sample_count = samples_to_fill;
            sound_from_game.samples_per_second = sound_output.samples_per_second;
            spear_engine_game_sound(&g_engine, &sound_from_game);

            sound_output.samples_filled_count += samples_to_fill;
            if (sound_output.is_playing == false)
            {
                sound_output.is_playing = true;
                SDL_PauseAudio(0);
            }
        }

        SDL_GL_SwapWindow(window);

        timepoint end_of_frame = platform_clock_now();
        last_frame_dt = get_duration_between_timepoints(last_timepoint, end_of_frame);
        last_timepoint = end_of_frame;
    }

    SDL_GL_DeleteContext(render_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

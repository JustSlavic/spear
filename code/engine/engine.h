#ifndef _SPEAR_ENGINE_H
#define _SPEAR_ENGINE_H

#include <corelibs/base.h>
#include <corelibs/memory/allocator.h>
#include <corelibs/platform.h>
#include <corelibs/bitmap.h>
#include <gamelibs/input.h>
#include "graphics/render.h"
#include "audio/audio.h"
#include "game_interface.h"


typedef struct
{
    memory_view memory;
    memory_view game_memory;
    memory_allocator allocator;
    memory_allocator temporary;

    bool32 running;

    /* Dynamic loading and hot reload */
    struct dll *game_dll;
    timestamp_t game_dll_timestamp;
    initialize_memory_t *initialize_memory;
    update_and_render_t *update_and_render;

    /* Arguments to the game */
    context game_context;
    input input;

    /* Rendering */
    renderer renderer;

    int32 current_client_width;
    int32 current_client_height;

    bool32 viewport_changed;
    viewport viewport;

    float fov;
    float aspect_ratio;
    float near_clip_width;
    float near_clip_height;
    float near_clip_distance;
    float far_clip_distance;

    gpu_mesh mesh_square;
    gpu_mesh mesh_square_uv;
    gpu_mesh mesh_tetrahedron;
    gpu_mesh mesh_cube;
    gpu_mesh mesh_octahedron;
    gpu_mesh mesh_icosahedron;
    gpu_mesh mesh_sphere;
    gpu_mesh mesh_ico_sphere;
    gpu_mesh mesh_ui_frame;
    gpu_shader shader_single_color;
    gpu_shader shader_textured;
    gpu_shader shader_ground;
    gpu_shader shader_framebuffer;
    gpu_shader shader_text;
    gpu_shader shader_phong;
    gpu_shader shader_sun;
    gpu_shader shader_ui_frame;

    audio_buffer master_audio;

    audio_sine_wave sine_audio_262Hz;
    audio_sine_wave sine_audio_500Hz;
    audio_buffer bird_audio;
    double audio_latency;

    uint32 sound_debug_position_count;
    uint32 sound_debug_position_running_index;
    float *sound_debug_positions_read;
    float *sound_debug_positions_write;
    float *sound_debug_positions_latency;

    bitmap test_bmp;
    gpu_texture test_tx;
} spear_engine;

void spear_engine_init(spear_engine *engine);
void spear_engine_init_graphics(spear_engine *engine);
void spear_engine_create_meshes(spear_engine *engine);
void spear_engine_compile_shaders(spear_engine *engine);
void spear_engine_load_game_dll(spear_engine *engine, char const *filename);
void spear_engine_input_reset_transitions(spear_engine *engine);
void spear_engine_input_mouse_pos_set(spear_engine *engine, int mouse_x, int mouse_y);
void spear_engine_update_viewport(spear_engine *engine, int width, int height);
void spear_engine_game_init(spear_engine *engine);
void spear_engine_game_update(spear_engine *engine);
void spear_engine_game_render(spear_engine *engine);


#endif // _SPEAR_ENGINE_H

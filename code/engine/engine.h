#ifndef _SPEAR_ENGINE_H
#define _SPEAR_ENGINE_H

#include <corelibs/base.h>
#include <corelibs/platform.h>
#include <gamelibs/input.h>
#include "graphics/render.h"
#include "game_interface.h"


typedef struct
{
    memory_view memory;
    memory_view game_memory;
    memory_allocator allocator;
    memory_allocator temporary;

    bool32 running;
    bool32 viewport_changed;
    int32 current_client_width;
    int32 current_client_height;

    /* Dynamic loading and hot reload */
    dll *game_dll;
    timestamp_t game_dll_timestamp;
    initialize_memory_t *initialize_memory;
    update_and_render_t *update_and_render;

    /* Arguments to the game */
    context game_context;
    input input;

    /* Rendering */
    float fov;
    float aspect_ratio;
    float near_clip_width;
    float near_clip_height;
    float near_clip_distance;
    float far_clip_distance;

    renderer renderer;
    gpu_mesh mesh_square;
    gpu_mesh mesh_tetrahedron;
    gpu_mesh mesh_cube;
    gpu_mesh mesh_octahedron;
    gpu_mesh mesh_icosahedron;
    gpu_mesh mesh_sphere;
    gpu_mesh mesh_ico_sphere;
    gpu_mesh mesh_ui_frame;
    gpu_shader shader_single_color;
    gpu_shader shader_ground;
    gpu_shader shader_framebuffer;
    gpu_shader shader_text;
    gpu_shader shader_phong;
    gpu_shader shader_sun;
    gpu_shader shader_ui_frame;
} engine;

void spear_engine_init(engine *engine);
void spear_engine_create_meshes(engine *engine);
void spear_engine_compile_shaders(engine *engine);
void spear_engine_load_game_dll(engine *engine);
void spear_engine_input_reset_transitions(engine *engine);
void spear_engine_update_viewport(engine *engine, int width, int height);
void spear_engine_game_init(engine *engine);
void spear_engine_game_update(engine *engine);
void spear_engine_game_render(engine *engine);


#endif // _SPEAR_ENGINE_H

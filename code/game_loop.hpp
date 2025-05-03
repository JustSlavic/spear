#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <base.h>
#include <context.hpp>
#include <common_graphics.hpp>
#include <debug_graph.hpp>


struct game_loop_data
{
    context ctx;

    memory_buffer memory;
    memory_buffer game_memory;

    memory_allocator allocator;
    memory_allocator temporary_allocator;

    bool32 is_running;
    uint64 frame_counter;

    gpu_mesh mesh_square;
    gpu_mesh mesh_square_uv;
    gpu_mesh mesh_tetrahedron;
    gpu_mesh mesh_cube;
    gpu_mesh mesh_octahedron;
    gpu_mesh mesh_icosahedron;
    gpu_mesh mesh_sphere;

    shader shader_color;
    shader shader_ground;
    shader shader_framebuffer;
    shader shader_text;
    shader shader_phong;
    shader shader_sun;

#if DEBUG
    debug_graph debug_graph_fps;
    bool32 is_debug_graph_fps_active;
#endif // DEBUG
};


void init_loop(game_loop_data *data);
void iter_loop(game_loop_data *data);


#endif // GAME_LOOP_HPP

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <base.h>
#include <math/matrix4.hpp>
#include <memory/allocator.hpp>
#include <console.hpp>
#include <array.hpp>

#include "execution_command.hpp"
#include "renderer_command.hpp"

#include "gfx/viewport.hpp"


typedef memory_buffer debug_load_file_t(memory_allocator allocator, char const *filename);

struct context
{
    memory_allocator temporary_allocator;

    void *game_state;

    uint32 window_width;
    uint32 window_height;

    gfx::viewport viewport;

    float32 aspect_ratio;
    float32 near_clip_dist;
    float32 near_clip_width;
    float32 near_clip_height;
    float32 far_clip_dist;

    static_array<exec_command, 32> exec_commands;
    static_array<rend_command, 1<<10> rend_commands;
    static_array<rend_command, 1<<10> rend_commands_ui;

    debug_load_file_t *debug_load_file;

    void exit_game();
    
#if DEBUG
    void debug_draw_off();
    void debug_draw_fps();
#endif

    void setup_camera(vector3 p, vector3 f, vector3 u);

    void render_mesh(render_mesh_tag mesh_tag, render_shader_tag shader_tag, vector3 position, vector3 scale, quaternion orientation = quaternion::identity(), vector4 color = V4(1.f));

    void render_square(matrix4 m, vector4 c, render_shader_tag s);
    void render_cube(matrix4 m, vector4 c, render_shader_tag s);
    void render_ui(matrix4 m, vector4 c);
    void render_banner(vector3 p, matrix4 m, vector4 c);
    void render_text(matrix4 m, vector4 c, char const *cstr);
    void render_planet(vector3 p, float32 r, vector4 c, quaternion q, render_shader_tag shader_tag);
};


#endif // CONTEXT_HPP

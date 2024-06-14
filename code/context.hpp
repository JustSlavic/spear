#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <base.h>
#include <math/matrix4.hpp>
#include <memory/allocator.hpp>

#include "execution_command.hpp"
#include "renderer_command.hpp"

#include "gfx/viewport.hpp"


typedef memory_buffer debug_load_file_t(memory_allocator allocator, char const *filename);

struct context
{
    memory_allocator temporary_allocator;
    memory_allocator resource_allocator;
    memory_allocator renderer_allocator;

    void *game_state;

    uint32 screen_width;
    uint32 screen_height;

    uint32 window_width;
    uint32 window_height;

    gfx::viewport viewport;

    float32 aspect_ratio;
    float32 near_clip_dist;
    float32 near_clip_width;
    float32 near_clip_height;
    float32 far_clip_dist;

    static_array<exec_command, 10> exec_commands;
    static_array<rend_command, 100> rend_commands;

    debug_load_file_t *debug_load_file;

    void exit_game();

    void setup_camera(vector3 p, vector3 f, vector3 u);

    void render_square(matrix4 m, vector4 c, shader_enum s);
    void render_cube(matrix4 m, vector4 c, shader_enum s);
    void render_ui(matrix4 m, vector4 c);
};


#endif // CONTEXT_HPP

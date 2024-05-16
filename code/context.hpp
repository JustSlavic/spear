#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <base.h>
#include <math/matrix4.hpp>
#include <memory/allocator.hpp>

#include "execution_command.hpp"
#include "renderer_command.hpp"

#include "rs/resource_system.hpp"


typedef memory_buffer debug_load_file_t(memory_allocator allocator, char const *filename);

struct context
{
    memory_allocator temporary_allocator;
    memory_allocator resource_allocator;
    memory_allocator renderer_allocator;

    string_id::storage *strids;
    rs::storage *rs;
    void *game_state;

    uint32 screen_width;
    uint32 screen_height;

    uint32 window_width;
    uint32 window_height;

    uint32 letterbox_width;
    uint32 letterbox_height;

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
    void render_mesh(matrix4 model, rs::token mesh, rs::token shader, vector4 color);
    void render_mesh_texture(matrix4 model, rs::token mesh, rs::token shader, rs::token texture);
    void render_text(rs::token font_texture, rs::token mesh, rs::token shader, string_view text, vector4 color);
};


#endif // CONTEXT_HPP

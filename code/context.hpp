#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <base.h>
#include <matrix4.hpp>
#include <memory_allocator.hpp>

#include "execution_command.hpp"
#include "renderer_command.hpp"

#include "rs/resource_system.hpp"


struct context
{
    memory_allocator temporary_allocator;

    rs::storage *rs;
    void *game_state;

    static_array<exec_command, 10> exec_commands;
    static_array<rend_command, 100> rend_commands;

    void exit_game();

    void setup_camera(vector3 p, vector3 f, vector3 u);
    void render_mesh(matrix4 model, rs::token mesh, rs::token shader, vector4 color);
};


#endif // CONTEXT_HPP

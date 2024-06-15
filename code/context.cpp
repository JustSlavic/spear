#include "context.hpp"


void context::exit_game()
{
    exec_commands.push_back(exec_command{ exec_command::exit });
}

void context::setup_camera(vector3 p, vector3 f, vector3 u)
{
    rend_command cmd = { rend_command::setup_camera };
    cmd.position = p;
    cmd.forward = f;
    cmd.up = u;

    rend_commands.push_back(cmd);
}

void context::render_square(matrix4 m, vector4 c, shader_enum s)
{
    rend_command cmd = { rend_command::render_square };
    cmd.model = m;
    cmd.color = c;
    cmd.shader = s;
    rend_commands.push_back(cmd);
}

void context::render_cube(matrix4 m, vector4 c, shader_enum s)
{
    rend_command cmd = { rend_command::render_cube };
    cmd.model = m;
    cmd.color = c;
    cmd.shader = s;
    rend_commands.push_back(cmd);
}

void context::render_ui(matrix4 m, vector4 c)
{
    rend_command cmd = { rend_command::render_ui };
    cmd.model = m;
    cmd.color = c;
    rend_commands_ui.push_back(cmd);
}


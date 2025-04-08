#include "context.hpp"


void context::exit_game()
{
    exec_commands.push_back(exec_command{ exec_command::exit });
}

void context::setup_camera(vector3 p, vector3 f, vector3 u)
{
    rend_command cmd = { RenderCommand_SetupCamera };
    cmd.position = p;
    cmd.forward = f;
    cmd.up = u;

    rend_commands.push_back(cmd);
}

void context::render_square(matrix4 m, vector4 c, shader_enum s)
{
    rend_command cmd = { RenderCommand_RenderSquare };
    cmd.model = m;
    cmd.color = c;
    cmd.shader = s;
    rend_commands.push_back(cmd);
}

void context::render_cube(matrix4 m, vector4 c, shader_enum s)
{
    rend_command cmd = { RenderCommand_RenderCube };
    cmd.model = m;
    cmd.color = c;
    cmd.shader = s;
    rend_commands.push_back(cmd);
}

void context::render_ui(matrix4 m, vector4 c)
{
    rend_command cmd = { RenderCommand_RenderUi };
    cmd.model = m;
    cmd.color = c;
    rend_commands_ui.push_back(cmd);
}

void context::render_banner(vector3 p, matrix4 m, vector4 c)
{
    rend_command cmd = { RenderCommand_RenderBanner };
    cmd.position = p;
    cmd.model = m;
    cmd.color = c;
    rend_commands_ui.push_back(cmd);
}

void context::render_text(matrix4 m, vector4 c, char const *cstr)
{
    rend_command cmd = { RenderCommand_RenderText };
    cmd.model = m;
    cmd.color = c;
    cmd.cstr = cstr;
    rend_commands_ui.push_back(cmd);
}

void context::render_planet(vector3 p, float32 r, vector4 c, quaternion q)
{
    rend_command cmd = { RenderCommand_RenderSphere };
    cmd.position = p;
    cmd.scale = r;
    cmd.color = c;
    cmd.quat = q;
    rend_commands.push_back(cmd);
}

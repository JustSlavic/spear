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

void context::render_mesh(render_mesh_tag mesh_tag,
                          render_shader_tag shader_tag,
                          vector3 position,
                          vector3 scale,
                          quaternion orientation,
                          vector4 color)
{
    rend_command cmd = { RenderCommand_RenderMesh };
    cmd.shader_tag = shader_tag;
    cmd.mesh_tag = mesh_tag;
    cmd.position = position;
    cmd.scale = scale;
    cmd.orientation = orientation;
    cmd.color = color;

    rend_commands.push_back(cmd);
}

void context::render_square(matrix4 m, vector4 c, render_shader_tag s)
{
    rend_command cmd = { RenderCommand_RenderSquare };
    cmd.model = m;
    cmd.color = c;
    cmd.shader_tag = s;
    cmd.mesh_tag = RenderMesh_Square;
    rend_commands.push_back(cmd);
}

void context::render_cube(matrix4 m, vector4 c, render_shader_tag s)
{
    rend_command cmd = { RenderCommand_RenderCube };
    cmd.model = m;
    cmd.color = c;
    cmd.shader_tag = s;
    cmd.mesh_tag = RenderMesh_Cube;
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

void context::render_planet(vector3 p, float32 r, vector4 c, quaternion q, render_shader_tag shader_tag)
{
    render_mesh(RenderMesh_Sphere, shader_tag,
        p, V3(r), q, c);
}

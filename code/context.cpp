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

void context::render_mesh(matrix4 model, rs::token mesh, rs::token shader, vector4 color)
{
    rend_command cmd = { rend_command::render_mesh_single_color };
    cmd.model = model;
    cmd.mesh_token = mesh;
    cmd.shader_token = shader;
    cmd.color = color;

    rend_commands.push_back(cmd);
}

void context::render_mesh_texture(matrix4 model, rs::token mesh, rs::token shader, rs::token texture)
{
    rend_command cmd = { rend_command::render_mesh_texture };
    cmd.model = model;
    cmd.mesh_token = mesh;
    cmd.shader_token = shader;
    cmd.texture_token = texture;

    rend_commands.push_back(cmd);
}

void context::render_text(rs::token font_texture, rs::token mesh, rs::token shader, string_view text, vector4 color)
{
    rend_command cmd = { rend_command::render_text };
    cmd.mesh_token = mesh;
    cmd.shader_token = shader;
    cmd.texture_token = font_texture;
    cmd.text = text;
    cmd.color = color;

    rend_commands.push_back(cmd);
}

void context::render_ui(matrix4 model, rs::token mesh, rs::token shader, vector4 color)
{
    rend_command cmd = { rend_command::render_ui_single_color };
    cmd.model = model;
    cmd.mesh_token = mesh;
    cmd.shader_token = shader;
    cmd.color = color;

    rend_commands.push_back(cmd);
}



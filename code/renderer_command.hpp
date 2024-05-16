#ifndef RENDERER_COMMAND_HPP
#define RENDERER_COMMAND_HPP

#include <base.h>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>
#include <rs/resource_system.hpp>


struct rend_command
{
    enum rend_command_kind
    {
        setup_camera,
        render_mesh_single_color,
        render_mesh_texture,
        render_text,
    };
    rend_command_kind kind;

    vector3 position;
    vector3 forward;
    vector3 up;

    matrix4 model;

    rs::token mesh_token;
    rs::token shader_token;
    rs::token texture_token;

    vector4 color;
    string_view text;
};

#endif // RENDERER_COMMAND_HPP

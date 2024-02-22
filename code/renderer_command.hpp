#ifndef RENDERER_COMMAND_HPP
#define RENDERER_COMMAND_HPP

#include <base.h>
#include <vector4.hpp>
#include <matrix4.hpp>
#include <rs/resource_system.hpp>


struct rend_command
{
    enum rend_command_kind
    {
        setup_camera,
        render_mesh_single_color,
    };
    rend_command_kind kind;

    union
    {
        struct
        {
            vector3 position;
            vector3 forward;
            vector3 up;
        };
        struct
        {
            matrix4 model;
            rs::token mesh_token;
            rs::token shader_token;
            vector4 color;
        };
    };
};

#endif // RENDERER_COMMAND_HPP

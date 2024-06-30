#ifndef RENDERER_COMMAND_HPP
#define RENDERER_COMMAND_HPP

#include <base.h>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>


enum shader_enum
{
    SHADER_NONE,
    SHADER_COLOR,
    SHADER_GROUND,
};

struct rend_command
{
    enum rend_command_kind
    {
        setup_camera,
        render_square,
        render_cube,
        render_ui,
        render_banner,
        render_text,
    };
    rend_command_kind kind;

    vector3 position;
    vector3 forward;
    vector3 up;

    matrix4 model;
    vector4 color;
    shader_enum shader;
    char const *cstr;
};

#endif // RENDERER_COMMAND_HPP

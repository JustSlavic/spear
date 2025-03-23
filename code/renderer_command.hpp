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

enum render_command_tag
{
    RenderCommand_SetupCamera,
    RenderCommand_RenderSquare,
    RenderCommand_RenderCube,
    RenderCommand_RenderUi,
    RenderCommand_RenderBanner,
    RenderCommand_RenderText,

    RenderCommand_RenderSphere,
};

struct rend_command
{
    render_command_tag tag;

    vector3 position;
    vector3 forward;
    vector3 up;
    float32 scale;

    matrix4 model;
    vector4 color;
    shader_enum shader;
    char const *cstr;
};

#endif // RENDERER_COMMAND_HPP

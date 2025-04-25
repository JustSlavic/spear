#ifndef RENDERER_COMMAND_HPP
#define RENDERER_COMMAND_HPP

#include <base.h>
#include <math.hpp>


enum render_command_tag
{
    RenderCommand_SetupCamera,
    RenderCommand_RenderMesh,
    RenderCommand_RenderSquare,
    RenderCommand_RenderCube,
    RenderCommand_RenderUi,
    RenderCommand_RenderBanner,
    RenderCommand_RenderText,

    RenderCommand_RenderSphere,
};

enum render_shader_tag
{
    RenderShader_None,
    RenderShader_SingleColor,
    RenderShader_Ground,
    RenderShader_Phong,
    RenderShader_Sun,
};

enum render_mesh_tag
{
    RenderMesh_None,
    RenderMesh_Square,
    RenderMesh_Cube,
    RenderMesh_Sphere,
};

struct rend_command
{
    render_command_tag tag;
    render_shader_tag shader_tag;
    render_mesh_tag mesh_tag;

    vector3 position;
    vector3 forward;
    vector3 up;
    vector3 scale;
    quaternion orientation;

    matrix4 model;
    vector4 color;
    char const *cstr;
    matrix3 mo;
};

#endif // RENDERER_COMMAND_HPP

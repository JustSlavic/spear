#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <base.hpp>
#include <math/vector3.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>
#include <gfx/viewport.hpp>


namespace gfx
{

enum class graphics_api
{
    none,
    software,
    opengl,
    vulkan,
    direct3d11,
    direct3d12,
    metal,
};

struct render_command
{
    enum command_type
    {
        setup_projection_matrix,
        setup_camera,
        draw_rectangle,
    };

    command_type type;

    union
    {
        struct // setup_projection_matrix
        {
            math::matrix4 projection;
        };
        struct // setup_camera
        {
            math::vector3 camera_position;
            math::vector3 look_at_position;
            math::vector3 camera_up_direction;
        };
        struct // draw_rectangle
        {
            math::rectangle2 rect;
            math::matrix4 model;
        };
    };
};

void initialize(graphics_api api);
void vsync(bool32_t active);
void set_clear_color(float32 r, float32 g, float32 b, float32 a);
void clear();
void set_viewport(viewport vp);
math::matrix4 make_look_at_matrix(math::vector3 eye, math::vector3 at, math::vector3 up);
math::matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f);
math::matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f);
math::matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f);
math::matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f);
void setup_projection_matrix(render_command *cmd);
void setup_camera(render_command *cmd);
void draw_rectangle(render_command *cmd, math::matrix4 view, math::matrix4 projection);


} // namespace gfx


#endif // RENDERER_HPP
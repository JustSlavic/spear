#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <base.hpp>
#include <math/vector3.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>


namespace gfx
{

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


void setup_projection_matrix(render_command *cmd);
void setup_camera(render_command *cmd);
void draw_rectangle(render_command *cmd, math::matrix4 view, math::matrix4 projection);


} // namespace gfx


#endif // RENDERER_HPP

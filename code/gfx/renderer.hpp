#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <base.hpp>
#include <platform.hpp>
#include <math/vector3.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>
#include <gfx/viewport.hpp>
#include <gfx/vertex_buffer_layout.hpp>
#include <resource_system.hpp>


struct execution_context;


namespace gfx
{

enum class graphics_api
{
    none,
    software,
    opengl,
    vulkan,
    dx11,
    dx12,
    metal,
};

struct driver
{
    uint8 data[64];
};

struct render_command
{
    enum class command_type
    {
        setup_projection_matrix,
        setup_camera,
        draw_background,
        draw_mesh_1,
        draw_mesh_with_color,
    };
    struct command_setup_projection_matrix
    {
        math::matrix4 projection;
    };
    struct command_setup_camera
    {
        math::vector3 camera_position;
        math::vector3 look_at_position;
        math::vector3 camera_up_direction;
    };
    struct command_draw_background
    {
        rs::resource_token mesh;
        rs::resource_token shader;
        math::vector4 color;
    };
    struct command_draw_rectangle
    {
        math::rectangle2 rect;
        math::matrix4 model;
    };
    struct command_draw_mesh_1
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        math::matrix4 model;
    };
    struct command_draw_mesh_with_color
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        math::matrix4 model;
        math::vector4 color;
    };

    command_type type;
    union
    {
        command_setup_projection_matrix setup_projection_matrix;
        command_setup_camera setup_camera;
        command_draw_background draw_background;
        command_draw_rectangle draw_rectangle;
        command_draw_mesh_1 draw_mesh_1;
        command_draw_mesh_with_color draw_mesh_with_color;
    };
};

bool32 initialize_opengl(void *driver);
bool32 initialize_dx11(void *window, driver *driver);
void vsync(void *window, bool32 active);
void set_clear_color(float32 r, float32 g, float32 b, float32 a);
void clear();
void set_viewport(viewport vp);
void swap_buffers(void *window, void *driver);

math::matrix4 make_look_at_matrix(math::vector3 eye, math::vector3 at, math::vector3 up);
math::matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f);
math::matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f);
math::matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f);
math::matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f);

void setup_projection_matrix(render_command *cmd);
void setup_camera(render_command *cmd);

void draw_polygon_simple(execution_context *context,
                         rs::resource_token mesh_token,
                         rs::resource_token shader_token,
                         math::matrix4 model,
                         math::matrix4 view,
                         math::matrix4 projection,
                         math::vector4 color);

void draw_background(execution_context *context, render_command *cmd);


} // namespace gfx


#endif // RENDERER_HPP

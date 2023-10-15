#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <base.h>
#include <g3.hpp>
#include <g301.hpp>
#include <math/rectangle2.hpp>
#include <gfx/viewport.hpp>
#include <gfx/vertex_buffer_layout.hpp>
#include <rs/resource_system.hpp>
#include <execution_context.hpp>


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



bool32 initialize_opengl(void *driver);
bool32 initialize_dx11(void *window, driver *driver);
void destroy_window_and_driver(void *window, void *driver);
void vsync(void *window, bool32 active);
void set_clear_color(float32 r, float32 g, float32 b, float32 a);
void clear();
void set_viewport(viewport vp);
void swap_buffers(void *window, void *driver);

matrix4 make_look_at_matrix(vector3 eye, vector3 at, vector3 up);
matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f);
matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f);
matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f);
matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f);

void setup_projection_matrix(render_command *cmd);
void setup_camera(render_command *cmd);

void draw_background(execution_context *context, render_command *cmd);
void draw_polygon_simple(execution_context *context,
                         resource_token mesh_token,
                         resource_token shader_token,
                         matrix4 model,
                         matrix4 view,
                         matrix4 projection,
                         vector4 color);
void draw_rectangle_texture(execution_context *context,
                            resource_token mesh_token,
                            resource_token shader_token,
                            resource_token texture_token,
                            matrix4 model,
                            matrix4 view,
                            matrix4 projectoin);


} // namespace gfx


#endif // RENDERER_HPP

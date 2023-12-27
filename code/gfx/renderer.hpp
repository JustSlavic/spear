#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <base.h>
#include <math/rectangle2.hpp>
#include <gfx/viewport.hpp>
#include <gfx/vertex_buffer_layout.hpp>
#include <rs/resource_system.hpp>
#include <execution_context.hpp>


struct execution_context;


enum class gfx__api
{
    none,
    software,
    opengl,
    vulkan,
    dx11,
    dx12,
    metal,
};

struct gfx__driver
{
    uint8 data[64];
};



bool32 gfx__initialize_opengl(void *window, void *driver);
bool32 gfx__initialize_dx11(void *window, void *driver);
void gfx__destroy_window_and_driver(void *window, void *driver);
void gfx__vsync(void *window, bool32 active);
void gfx__set_clear_color(float32 r, float32 g, float32 b, float32 a);
void gfx__clear();
void gfx__depth_test(void *driver, bool32 do_depth_test);
void gfx__write_depth(void *driver, bool32 write_depth);
void gfx__set_viewport(viewport vp);
void gfx__swap_buffers(void *window, void *driver);

matrix4 gfx__make_look_at_matrix(vector3 eye, vector3 at, vector3 up);
matrix4 gfx__make_projection_matrix(float32 w, float32 h, float32 n, float32 f);
matrix4 gfx__make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f);
matrix4 gfx__make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f);
matrix4 gfx__make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f);

void gfx__setup_projection_matrix(render_command *cmd);
void gfx__setup_camera(render_command *cmd);

void gfx__draw_background(execution_context *context, render_command *cmd);
void gfx__draw_polygon_simple(execution_context *context,
                         resource_token mesh_token,
                         resource_token shader_token,
                         matrix4 model,
                         matrix4 view,
                         matrix4 projection,
                         vector4 color);
void gfx__draw_rectangle_texture(execution_context *context,
                            resource_token mesh_token,
                            resource_token shader_token,
                            resource_token texture_token,
                            matrix4 model,
                            matrix4 view,
                            matrix4 projectoin);



#endif // RENDERER_HPP

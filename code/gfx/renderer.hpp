#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <base.h>
#include <math/vector3.hpp>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>
#include <rs/resource_system.hpp>
#include "viewport.hpp"


namespace gfx
{

enum class api
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
    api m_api;
    uint8 data[64];

    static driver initialize_opengl();

    void clear_color(float32 r, float32 g, float32 b, float32 a);
    void clear_color(vector4 color);
    void clear();
    void set_viewport(viewport vp);
    void depth_test(bool do_test);
    void write_depth(bool do_write);
    void vsync(bool do_vsync);
    void swap_buffers(void *w);

    matrix4 make_look_at_matrix(vector3 eye, vector3 at, vector3 up);
    matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f);
    matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f);
    matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f);
    matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f);

    void render_mesh_single_color(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh, rs::token shader, vector4 color);
    void render_mesh_texture(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh, rs::token shader, rs::token texture);
    void render_text(context *ctx, matrix4 proj, rs::token mesh_token, rs::token shader_token, rs::token texture_token, string_view text, vector4 color);
};


} // namespace gfx





// bool32 gfx__initialize_opengl(void *window, void *driver);
// bool32 gfx__initialize_dx11(void *window, void *driver);
// void gfx__destroy_window_and_driver(void *window, void *driver);
// void gfx__vsync(void *window, bool32 active);
// void gfx__set_clear_color(float32 r, float32 g, float32 b, float32 a);
// void gfx__clear();
// void gfx__depth_test(void *driver, bool32 do_depth_test);
// void gfx__write_depth(void *driver, bool32 write_depth);
// void gfx__set_viewport(viewport vp);
// void gfx__swap_buffers(void *window, void *driver);

// void gfx__setup_projection_matrix(render_command *cmd);
// void gfx__setup_camera(render_command *cmd);

// void gfx__draw_background(execution_context *context, render_command *cmd);
// void gfx__draw_polygon_simple(execution_context *context,
//                          resource_token mesh_token,
//                          resource_token shader_token,
//                          matrix4 model,
//                          matrix4 view,
//                          matrix4 projection,
//                          vector4 color);
// void gfx__draw_rectangle_texture(execution_context *context,
//                             resource_token mesh_token,
//                             resource_token shader_token,
//                             resource_token texture_token,
//                             matrix4 model,
//                             matrix4 view,
//                             matrix4 projectoin);



#endif // RENDERER_HPP

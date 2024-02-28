#include "renderer.hpp"
#include "gl.hpp"
#include "renderer_opengl.hpp"


namespace gfx
{

driver driver::initialize_opengl()
{
    driver result = {};

    bool32 ok = gl::initialize();
    if (ok)
    {
        result.m_api = api::opengl;
    }
    return result;
}

void driver::clear_color(float32 r, float32 g, float32 b, float32 a)
{
    if (m_api == api::opengl)
        gl::clear_color(r, g, b, a);
}

void driver::clear_color(vector4 color)
{
    if (m_api == api::opengl)
        gl::clear_color(color.r, color.g, color.b, color.a);
}

void driver::clear()
{
    if (m_api == api::opengl)
        gl::clear();
}

void driver::set_viewport(viewport vp)
{
    if (m_api == api::opengl)
        gl::set_viewport(vp);
}

void driver::depth_test(bool do_test)
{
    if (m_api == api::opengl) gl::depth_test(do_test);
}

void driver::write_depth(bool do_write)
{
    if (m_api == api::opengl) gl::write_depth(do_write);
}

void driver::vsync(bool do_vsync)
{
    if (m_api == api::opengl) gl::vsync(do_vsync);
}

void driver::swap_buffers(void *w)
{
    if (m_api == api::opengl)
        gl::swap_buffers(w);
}

matrix4 driver::make_look_at_matrix(vector3 eye, vector3 at, vector3 up)
{
    vector3 f = normalized(at - eye);
    vector3 s = normalized(cross(f, up));
    vector3 u = cross(s, f);

    matrix4 result = {};
    result._1 = V4(s, -inner(s, eye));
    result._2 = V4(u, -inner(u, eye));
    result._3 = V4(-f, inner(f, eye));
    result._4 = V4(0, 0, 0, 1);

    return result;
}

matrix4 driver::make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};
    if (m_api == api::opengl)
        result = gl::make_projection_matrix(w, h, n, f);
    return result;
}

matrix4 driver::make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result = {};
    if (m_api == api::opengl)
        result = gl::make_projection_matrix_fov(fov, aspect_ratio, n, f);
    return result;
}

matrix4 driver::make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};
    if (m_api == api::opengl)
        result = gl::make_orthographic_matrix(w, h, n, f);
    return result;
}

matrix4 driver::make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result = {};
    if (m_api == api::opengl)
        result = gl::make_orthographic_matrix(aspect_ratio, n, f);
    return result;
}


void driver::render_mesh_single_color(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh, rs::token shader, vector4 color)
{
    if (m_api == api::opengl)
        gl::render_mesh_single_color(ctx, model, view, proj, mesh, shader, color);
}


void driver::render_mesh_texture(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh, rs::token shader, rs::token texture)
{
    if (m_api == api::opengl)
        gl::render_mesh_texture(ctx, model, view, proj, mesh, shader, texture);
}


void driver::render_text(context *ctx, matrix4 proj, rs::token mesh_token, rs::token shader_token, rs::token texture_token, string_view text, vector4 color)
{
    if (m_api == api::opengl)
        gl::render_text(ctx, proj, mesh_token, shader_token, texture_token, text, color);
}


} // namespace gfx


#include "renderer_opengl.cpp"







// #include "renderer_opengl.cpp"


// GLOBAL gfx__api gfx__active_api;




// void gfx__destroy_window_and_driver(void *window, void *driver)
// {
//     if (gfx__active_api == gfx__api::opengl)
//         gl__destroy_window_and_driver(window, driver);
// }

// void gfx__draw_background(execution_context *context, render_command *cmd)
// {
//     gfx__draw_polygon_simple(context,
//                              cmd->draw_background.mesh,
//                              cmd->draw_background.shader,
//                              matrix4::identity(),
//                              matrix4::identity(),
//                              matrix4::identity(),
//                              cmd->draw_background.color);
// }

// void gfx__draw_polygon_simple(execution_context *context,
//                               resource_token mesh_token,
//                               resource_token shader_token,
//                               matrix4 model,
//                               matrix4 view,
//                               matrix4 projection,
//                               vector4 color)
// {
//     ASSERT(mesh_token.type == RESOURCE_MESH);
//     ASSERT(shader_token.type == RESOURCE_SHADER);

//     if (mesh_token.type == RESOURCE_MESH)
//     {
//         resource__mesh *mesh = get_mesh_resource(mesh_token);
//         if (shader_token.type == RESOURCE_SHADER)
//         {
//             resource__shader *shader = get_shader_resource(shader_token);
//             if (!mesh->render_data.data)
//             {
//                 gl__load_mesh(context, mesh);
//             }
//             if (!shader->render_data.data)
//             {
//                 gl__load_shader(context, shader);
//             }
//             gl__draw_indexed_triangles(mesh, shader, model, view, projection, color);
//         }
//         else
//         {
//             // @todo: shader resource is not valid
//         }
//     }
//     else
//     {
//         // @todo: mesh resource is not valid
//     }
// }

// void gfx__draw_rectangle_texture(execution_context *context,
//                                  resource_token mesh_token,
//                                  resource_token shader_token,
//                                  resource_token texture_token,
//                                  matrix4 model,
//                                  matrix4 view,
//                                  matrix4 projection)
// {
//     if (mesh_token.type == RESOURCE_MESH)
//     {
//         resource__mesh *mesh = get_mesh_resource(mesh_token);
//         if (shader_token.type == RESOURCE_SHADER)
//         {
//             resource__shader *shader = get_shader_resource(shader_token);
//             if (texture_token.type == RESOURCE_TEXTURE)
//             {
//                 resource__texture *texture = get_texture_resource(texture_token);
//                 if (!mesh->render_data.data)
//                 {
//                     gl__load_mesh(context, mesh);
//                 }
//                 if (!shader->render_data.data)
//                 {
//                     gl__load_shader(context, shader);
//                 }
//                 if (!texture->render_data.data)
//                 {
//                     gl__load_texture(context, texture);
//                 }
//                 gl__draw_indexed_triangles(mesh, shader, texture, model, view, projection);
//             }
//         }
//         else
//         {
//             // @todo: shader resource is not valid
//         }
//     }
//     else
//     {
//         // @todo: mesh resource is not valid
//     }
// }

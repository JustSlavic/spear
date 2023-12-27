#include "renderer.hpp"
#include "renderer_opengl.cpp"

#if OS_WINDOWS
#include "renderer_dx11.cpp"
#endif

// @todo:
// #include <renderer_direct3d12.cpp>
// #include <renderer_vulkan.cpp>
// #include <renderer_metal.cpp>


GLOBAL gfx__api gfx__active_api;


bool32 gfx__initialize_opengl(void *window, void *driver)
{
    UNUSED(window);
    UNUSED(driver);

    bool32 result = gl__initialize();
    if (result) gfx__active_api = gfx__api::opengl;
    return result;
}

void gfx__destroy_window_and_driver(void *window, void *driver)
{
    if (gfx__active_api == gfx__api::opengl)
        gl__destroy_window_and_driver(window, driver);
}

void gfx__vsync(void *window, bool32 active)
{
    if (gfx__active_api == gfx__api::opengl)
        gl__vsync(window, active);
}

void gfx__set_clear_color(float32 r, float g, float b, float a)
{
    if (gfx__active_api == gfx__api::opengl)
        gl__set_clear_color(r, g, b, a);
}

void gfx__clear()
{
    if (gfx__active_api == gfx__api::opengl)
        gl__clear();
}

void gfx__depth_test(void *driver, bool32 do_depth_test)
{
    if (gfx__active_api == gfx__api::opengl)
        gl__depth_test(do_depth_test);
}

void gfx__write_depth(void *driver, bool32 write_depth)
{
    if (gfx__active_api == gfx__api::opengl)
        gl__write_depth(write_depth);
}

void gfx__set_viewport(viewport vp)
{
    if (gfx__active_api == gfx__api::opengl)
        gl__set_viewport(vp);
    else
        ASSERT_FAIL("NOT IMPLEMENTED");
}

void gfx__swap_buffers(void *wnd, void *drv)
{
    if (gfx__active_api == gfx__api::opengl)
    {
        gl__swap_buffers(wnd);
    }
    else if (gfx__active_api == gfx__api::dx11)
    {
        // dx::swap_buffers(d);
        ASSERT_FAIL();
    }
}

matrix4 gfx__make_look_at_matrix(vector3 eye, vector3 at, vector3 up)
{
    using namespace math;

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

matrix4 gfx__make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};
    if (gfx__active_api == gfx__api::opengl)
        result = gl__make_projection_matrix(w, h, n, f);
    return result;
}

matrix4 gfx__make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result = {};
    if (gfx__active_api == gfx__api::opengl)
        result = gl__make_projection_matrix_fov(fov, aspect_ratio, n, f);
    return result;
}

matrix4 gfx__make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};
    if (gfx__active_api == gfx__api::opengl)
        result = gl__make_orthographic_matrix(w, h, n, f);
    return result;
}

matrix4 gfx__make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result = {};
    if (gfx__active_api == gfx__api::opengl)
        result = gl__make_orthographic_matrix(aspect_ratio, n, f);
    return result;
}

void gfx__draw_background(execution_context *context, render_command *cmd)
{
    gfx__draw_polygon_simple(context,
                             cmd->draw_background.mesh,
                             cmd->draw_background.shader,
                             matrix4::identity(),
                             matrix4::identity(),
                             matrix4::identity(),
                             cmd->draw_background.color);
}

void gfx__draw_polygon_simple(execution_context *context,
                              resource_token mesh_token,
                              resource_token shader_token,
                              matrix4 model,
                              matrix4 view,
                              matrix4 projection,
                              vector4 color)
{
    ASSERT(mesh_token.type == RESOURCE_MESH);
    ASSERT(shader_token.type == RESOURCE_SHADER);

    if (mesh_token.type == RESOURCE_MESH)
    {
        resource__mesh *mesh = get_mesh_resource(mesh_token);
        if (shader_token.type == RESOURCE_SHADER)
        {
            resource__shader *shader = get_shader_resource(shader_token);
            if (!mesh->render_data.memory)
            {
                gl__load_mesh(context, mesh);
            }
            if (!shader->render_data.memory)
            {
                gl__load_shader(context, shader);
            }
            gl__draw_indexed_triangles(mesh, shader, model, view, projection, color);
        }
        else
        {
            // @todo: shader resource is not valid
        }
    }
    else
    {
        // @todo: mesh resource is not valid
    }
}

void gfx__draw_rectangle_texture(execution_context *context,
                                 resource_token mesh_token,
                                 resource_token shader_token,
                                 resource_token texture_token,
                                 matrix4 model,
                                 matrix4 view,
                                 matrix4 projection)
{
    if (mesh_token.type == RESOURCE_MESH)
    {
        resource__mesh *mesh = get_mesh_resource(mesh_token);
        if (shader_token.type == RESOURCE_SHADER)
        {
            resource__shader *shader = get_shader_resource(shader_token);
            if (texture_token.type == RESOURCE_TEXTURE)
            {
                resource__texture *texture = get_texture_resource(texture_token);
                if (!mesh->render_data.memory)
                {
                    gl__load_mesh(context, mesh);
                }
                if (!shader->render_data.memory)
                {
                    gl__load_shader(context, shader);
                }
                if (!texture->render_data.memory)
                {
                    gl__load_texture(context, texture);
                }
                gl__draw_indexed_triangles(mesh, shader, texture, model, view, projection);
            }
        }
        else
        {
            // @todo: shader resource is not valid
        }
    }
    else
    {
        // @todo: mesh resource is not valid
    }
}

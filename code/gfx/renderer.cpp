#include "renderer.hpp"
#include "renderer_opengl.cpp"

#if OS_WINDOWS
#include "renderer_dx11.cpp"
#endif

// @todo:
// #include <renderer_direct3d12.cpp>
// #include <renderer_vulkan.cpp>
// #include <renderer_metal.cpp>


namespace gfx
{

GLOBAL graphics_api active_api;


bool32 initialize_opengl(void *d)
{
    bool32 result = gl::initialize();
    if (result) active_api = graphics_api::opengl;
    return result;
}

#if OS_WINDOWS
bool32 initialize_dx11(win32::window *w, driver *d)
{
    bool32 result = dx11::initialize(w, d);
    if (result) active_api = graphics_api::dx11;
    return result;
}
#endif // OS_WINDOWS

void destroy_window_and_driver(void *window, void *driver)
{
    if (active_api == graphics_api::opengl)
        gl::destroy_window_and_driver(window, driver);
}

void vsync(void *window, bool32 active)
{
    if (active_api == graphics_api::opengl)
        gl::vsync(window, active);
}

void set_clear_color(float32 r, float g, float b, float a)
{
    if (active_api == graphics_api::opengl)
        gl::set_clear_color(r, g, b, a);
}

void clear()
{
    if (active_api == graphics_api::opengl)
        gl::clear();
}

void set_viewport(viewport vp)
{
    if (active_api == graphics_api::opengl)
        gl::set_viewport(vp);
    else
        ASSERT_FAIL("NOT IMPLEMENTED");
}

void swap_buffers(void *wnd, void *drv)
{
    if (active_api == graphics_api::opengl)
    {
        gl::swap_buffers(wnd);
    }
    else if (active_api == graphics_api::dx11)
    {
        // dx::swap_buffers(d);
        ASSERT_FAIL();
    }
}

matrix4 make_look_at_matrix(vector3 eye, vector3 at, vector3 up)
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

matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_projection_matrix(w, h, n, f);
    return result;
}

matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_projection_matrix_fov(fov, aspect_ratio, n, f);
    return result;
}

matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_orthographic_matrix(w, h, n, f);
    return result;
}

matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_orthographic_matrix(aspect_ratio, n, f);
    return result;
}


void setup_projection_matrix(render_command *cmd)
{

}

void setup_camera(render_command *cmd)
{

}

void draw_background(execution_context *context, render_command *cmd)
{
    draw_polygon_simple(context,
                        cmd->draw_background.mesh,
                        cmd->draw_background.shader,
                        matrix4__identity(),
                        matrix4__identity(),
                        matrix4__identity(),
                        cmd->draw_background.color);
}

void draw_polygon_simple(execution_context *context,
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
                gl::load_mesh(context, mesh);
            }
            if (!shader->render_data.memory)
            {
                gl::load_shader(context, shader);
            }
            gl::draw_indexed_triangles(mesh, shader, model, view, projection, color);
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

void draw_rectangle_texture(execution_context *context,
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
                    gl::load_mesh(context, mesh);
                }
                if (!shader->render_data.memory)
                {
                    gl::load_shader(context, shader);
                }
                if (!texture->render_data.memory)
                {
                    gl::load_texture(context, texture);
                }
                gl::draw_indexed_triangles(mesh, shader, texture, model, view, projection);
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


} // namespace gfx

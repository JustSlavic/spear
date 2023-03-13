#include "renderer.hpp"
#include "renderer_opengl.cpp"

// @todo:
// #include <renderer_direct3d11.cpp>
// #include <renderer_direct3d12.cpp>
// #include <renderer_vulkan.cpp>
// #include <renderer_metal.cpp>


namespace gfx
{

GLOBAL graphics_api active_api;

void initialize(graphics_api api)
{
    if (api == graphics_api::opengl)
    {
        bool32 result = gl::initialize();
        if (result) active_api = graphics_api::opengl;
    }
    else
        ASSERT_FAIL("NOT IMPLEMENTED");
        // @todo:
        // void dx11::initialize();
        // void dx12::initialize();
        // void vk::initialize();
        // void mt::initialize();
}

void vsync(bool32 active)
{
    if (active_api == graphics_api::opengl)
        gl::vsync(active);
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

math::matrix4 make_look_at_matrix(math::vector3 eye, math::vector3 at, math::vector3 up)
{
    using namespace math;

    vector3 f = normalized(at - eye);
    vector3 s = normalized(cross(f, up));
    vector3 u = cross(s, f);

    matrix4 result =
    {
         s.x,  u.x,  -f.x, 0,
         s.y,  u.y,  -f.y, 0,
         s.z,  u.z,  -f.z, 0,
        -dot(s, eye), -dot(u, eye), dot(f, eye), 1,
    };

    return result;
}

math::matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    math::matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_projection_matrix(w, h, n, f);
    return result;
}

math::matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    math::matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_projection_matrix_fov(fov, aspect_ratio, n, f);
    return result;
}

math::matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    math::matrix4 result = {};
    if (active_api == graphics_api::opengl)
        result = gl::make_orthographic_matrix(w, h, n, f);
    return result;
}

math::matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    math::matrix4 result = {};
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

void draw_polygon_simple(execution_context *context,
                         rs::resource_token mesh_token,
                         rs::resource_token shader_token,
                         math::matrix4 model,
                         math::matrix4 view,
                         math::matrix4 projection,
                         math::vector4 color)
{
    rs::resource *mesh = rs::get_resource(&context->resource_storage, mesh_token);
    if (mesh->type == rs::resource_type::mesh)
    {
        rs::resource *shader = rs::get_resource(&context->resource_storage, shader_token);
        if (shader->type == rs::resource_type::shader)
        {
            if (!mesh->render_data)
            {
                gl::load_mesh(context, mesh);
            }
            if (!shader->render_data)
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

void draw_background(execution_context *context, render_command *cmd)
{
    draw_polygon_simple(context,
                        cmd->draw_background.mesh,
                        cmd->draw_background.shader,
                        math::matrix4::identity(),
                        math::matrix4::identity(),
                        math::matrix4::identity(),
                        cmd->draw_background.color);
}


} // namespace gfx

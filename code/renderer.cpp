#include <renderer.hpp>
#include <renderer_opengl.cpp>
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
        bool32_t result = gl::initialize();
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

void vsync(bool32_t active)
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

void draw_rectangle(render_command *cmd, math::matrix4 view, math::matrix4 projection)
{
    if (active_api == graphics_api::opengl)
        gl::draw_rectangle(cmd, view, projection);
}

} // namespace gfx

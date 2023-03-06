#include <renderer.hpp>
#include <renderer_opengl.cpp>
// @todo:
// #include <renderer_direct3d11.cpp>
// #include <renderer_direct3d12.cpp>
// #include <renderer_vulkan.cpp>
// #include <renderer_metal.cpp>


namespace gfx
{

void setup_projection_matrix(render_command *cmd)
{

}

void setup_camera(render_command *cmd)
{

}

void draw_rectangle(render_command *cmd, math::matrix4 view, math::matrix4 projection)
{
    gl::draw_rectangle(cmd, view, projection);
}

} // namespace gfx

#include "resource_system.hpp"


namespace rs
{


token create_mesh(storage *s, memory_buffer vbo, memory_buffer ibo, gfx::vertex_buffer_layout vbl)
{
    mesh m;
    m.vbo = mallocator().allocate_copy(vbo.data, vbo.size);
    m.ibo = mallocator().allocate_copy(ibo.data, ibo.size);
    m.vbl = vbl;
    m.render_data = NULL;

    s->meshes.push_back(m);

    token result;
    result.kind = resource_kind::mesh;
    result.index = (uint32) s->meshes.size() - 1;

    return result;
}

token create_shader(storage *s)
{
    shader sh = {};
    s->shaders.push_back(sh);

    token result;
    result.kind = resource_kind::shader;
    result.index = (uint32) s->shaders.size() - 1;

    return result;
}


mesh *storage::get_mesh(token t)
{
    auto result = meshes.data() + t.index;
    return result;
}

shader *storage::get_shader(token t)
{
    auto result = shaders.data() + t.index;
    return result;
}


} // namespace rs

#ifndef GFX_RESOURCE_HPP
#define GFX_RESOURCE_HPP

#include <base.hpp>
#include <gfx/vertex_buffer_layout.hpp>

namespace rs {


struct mesh_resource
{
    memory_block vbo;
    memory_block ibo;
    gfx::vertex_buffer_layout vbl;
};

struct texture_resource
{

};

struct shader_resource
{
    string_id name;
};

struct resource
{

    resource_type type;
    union
    {
        mesh_resource mesh;
        shader_resource shader;
    };
    void *render_data;
};


} // namespace rs

#endif // GFX_RESOURCE_HPP

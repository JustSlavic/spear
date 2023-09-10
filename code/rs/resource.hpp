#ifndef GFX_RESOURCE_HPP
#define GFX_RESOURCE_HPP

#include <base.h>
#include <gfx/vertex_buffer_layout.hpp>
#include <image/bitmap.hpp>


namespace rs {


struct mesh_resource
{
    memory_block vbo;
    memory_block ibo;
    gfx::vertex_buffer_layout vbl;
};

struct texture_resource
{
    image::bitmap texture;
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
        texture_resource texture;
    };
    void *render_data;
};


} // namespace rs

#endif // GFX_RESOURCE_HPP

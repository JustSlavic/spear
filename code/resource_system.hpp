#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <string_id.hpp>
#include <resource_token.hpp>
#include <gfx/renderer.hpp>


namespace rs
{


enum class resource_type
{
    mesh,
    shader,
};

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


// @todo: move this into cpp file, remove dependency on the renderer, by making variable-size array

struct resource_storage
{
    memory::allocator heap;

    // @todo: hash table string_id("name") -> pointer to resource in heap memory
    // if name's not provided, generate it randomly?
    // @todo: random?
    resource resources[32];
    uint32 resource_count;
};

resource_token get_new_resource_token(resource_storage *storage);
resource *get_resource(resource_storage *storage, resource_token token);
resource_token create_mesh_resource(resource_storage *storage, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl);
resource_token create_shader_resource(resource_storage *storage, string_id name);

} // namespace rs


#endif // RESOURCE_SYSTEM_HPP

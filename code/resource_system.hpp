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

struct resource_storage
{
    memory::allocator heap;

    // @todo: hash table string_id("name") -> pointer to resource in heap memory
    // if name's not provided, generate it randomly?
    // @todo: random?
    resource resources[32];
    uint32 resource_count;
};


INLINE resource_token get_new_resource_token(resource_storage *storage)
{
    ASSERT(storage->resource_count < ARRAY_COUNT(storage->resources));
    resource_token result = {storage->resource_count++};
    return result;
}

INLINE resource *get_resource(resource_storage *storage, resource_token token)
{
    resource *result = storage->resources + token.id;
    return result;
}

INLINE resource_token create_mesh_resource(resource_storage *storage, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl)
{
    resource_token token = get_new_resource_token(storage);
    if (token.id)
    {
        resource *r = get_resource(storage, token);
        r->type = rs::resource_type::mesh;
        r->mesh.vbo = ALLOCATE_COPY(&storage->heap, vbo);
        r->mesh.ibo = ALLOCATE_COPY(&storage->heap, ibo);
        r->mesh.vbl = vbl;
    }
    return token;
}

INLINE resource_token create_shader_resource(resource_storage *storage, string_id name)
{
    resource_token token = get_new_resource_token(storage);
    if (token.id)
    {
        resource *r = get_resource(storage, token);
        r->type = rs::resource_type::shader;
        r->shader.name = name;
    }
    return token;
}


} // namespace rs


#endif // RESOURCE_SYSTEM_HPP

#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <resource_token.hpp>
#include <gfx/renderer.hpp>


namespace rs
{


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

};

enum class resource_type
{
    mesh,
};


struct resource
{

    resource_type type;
    union
    {
        mesh_resource mesh;
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


INLINE resource_token create_mesh_resource(resource_storage *storage, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl)
{
    ASSERT(storage->resource_count < ARRAY_COUNT(storage->resources));
    uint32 id = storage->resource_count++;

    storage->resources[id].type = rs::resource_type::mesh;
    storage->resources[id].mesh.vbo = ALLOCATE_COPY(&storage->heap, vbo);
    storage->resources[id].mesh.ibo = ALLOCATE_COPY(&storage->heap, ibo);
    storage->resources[id].mesh.vbl = vbl;

    resource_token result = {id};
    return result;
}

INLINE resource *get_resource(resource_storage *storage, resource_token token)
{
    ASSERT(0 <= token.id && token.id < ARRAY_COUNT(storage->resources));

    resource *result = storage->resources + token.id;
    return result;
}


} // namespace rs


#endif // RESOURCE_SYSTEM_HPP

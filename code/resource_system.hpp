#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.hpp>
#include <memory/allocator.hpp>


namespace rs
{


struct resource_token
{
    uint32 id;
};


struct mesh_resource
{
    memory_block vbo;
    memory_block ibo;
};


struct texture_resource
{

};


struct shader_resource
{

};


struct resource
{
    bool32 is_loaded_on_videocard;
    union
    {
        mesh_resource mesh;
    };
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


INLINE resource_token create_mesh_resource(resource_storage *storage, memory_block vbo)
{
    ASSERT(storage->resource_count < ARRAY_COUNT(storage->resources));
    uint32 id = storage->resource_count++;

    storage->resources[id].mesh.vbo = ALLOCATE_COPY(&storage->heap, vbo);

    resource_token result = {id};
    return result;
}


} // namespace rs


#endif // RESOURCE_SYSTEM_HPP

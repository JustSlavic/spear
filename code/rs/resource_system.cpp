#include <rs/resource_system.hpp>
#include <rs/resource_token.hpp>
#include <rs/resource.hpp>


namespace rs {


resource_token get_new_resource_token(resource_storage *storage)
{
    ASSERT(storage->resources.size < storage->resources.capacity);
    resource_token result = resource_token{(uint32)storage->resources.size};
    storage->resources.push_back(resource{});
    return result;
}

resource *get_resource(resource_storage *storage, resource_token token)
{
    resource *result = storage->resources.data + token.id;
    return result;
}

resource_token create_null_resource(resource_storage *storage)
{
    resource_token token = get_new_resource_token(storage);
    return token;
}

resource_token create_mesh_resource(resource_storage *storage, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl)
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

resource_token create_shader_resource(resource_storage *storage, string_id name)
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

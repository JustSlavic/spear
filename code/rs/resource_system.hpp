#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.h>
#include <array.hpp>
#include <string_id.hpp>

#include <memory_allocator.h>
#include <rs/resource_token.hpp>
#include <gfx/vertex_buffer_layout.hpp>

#include <image/bitmap.hpp>



namespace rs {


enum class resource_type
{
    none,
    mesh,
    shader,
    texture,
};

struct resource;
struct resource_storage
{
    memory_allocator heap;
    array<resource> resources;
};

resource_token get_new_resource_token(resource_storage *storage);
resource *get_resource(resource_storage *storage, resource_token token);
resource_token create_null_resource(resource_storage *storage);
resource_token create_mesh_resource(resource_storage *storage, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl);
resource_token create_shader_resource(resource_storage *storage, string_id name);
resource_token create_texture_resource(resource_storage *storage, image::bitmap);


} // namespace rs


#endif // RESOURCE_SYSTEM_HPP

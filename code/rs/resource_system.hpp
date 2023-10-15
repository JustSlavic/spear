#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.h>
#include <array.hpp>
#include <string_id.hpp>

#include <memory_allocator.h>
#include <gfx/vertex_buffer_layout.hpp>

#include <image/bitmap.hpp>


struct resource_system;

enum resource_type : uint32
{
    RESOURCE_NONE,
    RESOURCE_MESH,
    RESOURCE_SHADER,
    RESOURCE_TEXTURE,
};

struct resource_token
{
    resource_system *system;
    resource_type type;
    uint32 index;
};

struct resource__mesh
{
    memory_block vbo;
    memory_block ibo;
    gfx::vertex_buffer_layout vbl;
    memory_block render_data;
};

struct resource__shader
{
    string_id name;
    memory_block render_data;
};

struct resource__texture
{
    bitmap texture;
    memory_block render_data;
};

struct resource_system
{
    memory_allocator heap;

    static_array<resource__mesh, 10> meshes;
    static_array<resource__shader, 10> shaders;
    static_array<resource__texture, 10> textures;
};

resource__mesh *get_mesh_resource(resource_token token);
resource__shader *get_shader_resource(resource_token token);
resource__texture *get_texture_resource(resource_token token);

resource_token create_mesh_resource(resource_system *system, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl);
resource_token create_shader_resource(resource_system *system, string_id name);
resource_token create_texture_resource(resource_system *system, bitmap texture);


#endif // RESOURCE_SYSTEM_HPP

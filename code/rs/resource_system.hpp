#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.h>
#include <gfx/vertex_buffer_layout.hpp>
#include <image/bitmap.hpp>
#include <string_id.hpp>


struct context;

namespace rs
{

enum class resource_kind
{
    none,
    mesh,
    shader,
    texture,
};

struct mesh
{
    memory_buffer vbo;
    memory_buffer ibo;
    gfx::vertex_buffer_layout vbl;
    void *render_data;
};

struct shader
{
    string_id name;
    void *render_data;
};

struct texture
{
    bitmap data;
    void *render_data;
};

struct token
{
    resource_kind kind;
    uint32 index;
};

struct storage
{
    static_array<mesh, 10> meshes;
    static_array<shader, 10> shaders;
    static_array<texture, 10> textures;

    mesh *get_mesh(token t);
    shader *get_shader(token t);
    texture *get_texture(token t);
};

token create_mesh(context *, storage *s, memory_buffer vbo, memory_buffer ibo, gfx::vertex_buffer_layout vbl);
token create_shader(context *, storage *s, string_id shader_name);
token create_texture(context *, storage *s, bitmap data);

token load_texture(context *ctx, storage *s, char const *filename);


} // namespace rs

#endif // RESOURCE_SYSTEM_HPP

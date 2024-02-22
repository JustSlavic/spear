#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.h>
#include <gfx/vertex_buffer_layout.hpp>


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
    // ???
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
    // static_array<resource__texture, 10> textures;

    mesh *get_mesh(token t);
    shader *get_shader(token t);
};

token create_mesh(storage *s, memory_buffer vbo, memory_buffer ibo, gfx::vertex_buffer_layout vbl);
token create_shader(storage *s);


} // namespace rs

#endif // RESOURCE_SYSTEM_HPP

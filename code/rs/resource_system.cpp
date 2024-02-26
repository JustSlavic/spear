#include "resource_system.hpp"
#include <context.hpp>

#include <image/png.hpp>
#include <util.hpp>


namespace rs
{


token create_mesh(context *ctx, storage *s, memory_buffer vbo, memory_buffer ibo, gfx::vertex_buffer_layout vbl)
{
    mesh m;
    m.vbo = ctx->resource_allocator.allocate_copy(vbo.data, vbo.size);
    m.ibo = ctx->resource_allocator.allocate_copy(ibo.data, ibo.size);
    m.vbl = vbl;
    m.render_data = NULL;

    s->meshes.push_back(m);

    token result;
    result.kind = resource_kind::mesh;
    result.index = (uint32) s->meshes.size() - 1;

    return result;
}

token create_shader(context *, storage *s, string_id shader_name)
{
    shader sh = {};
    sh.name = shader_name;
    s->shaders.push_back(sh);

    token result;
    result.kind = resource_kind::shader;
    result.index = (uint32) s->shaders.size() - 1;

    return result;
}

token create_texture(context *, storage *s, bitmap data)
{
    texture tx = {};
    tx.data = data;

    s->textures.push_back(tx);

    token result;
    result.kind = resource_kind::texture;
    result.index = (uint32) s->textures.size() - 1;

    return result;
}

mesh *storage::get_mesh(token t)
{
    auto result = meshes.data() + t.index;
    return result;
}

shader *storage::get_shader(token t)
{
    auto result = shaders.data() + t.index;
    return result;
}

texture *storage::get_texture(token t)
{
    auto result = textures.data() + t.index;
    return result;
}

token load_texture(context *ctx, storage *s, char const *filename)
{
    token result = {};

    memory_buffer file_content = ctx->debug_load_file(ctx->temporary_allocator, filename);
    if (file_content.data != NULL)
    {
        auto bitmap = image::load_png(ctx->resource_allocator, ctx->temporary_allocator, file_content);
        if (bitmap.pixels != NULL)
        {
            result = create_texture(ctx, s, bitmap);
        }
    }

    return result;
}


} // namespace rs

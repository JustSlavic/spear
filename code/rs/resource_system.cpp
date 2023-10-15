#include <rs/resource_system.hpp>


resource__mesh *get_mesh_resource(resource_token token)
{
    resource__mesh *result = token.system->meshes.data() + token.index;
    return result;
}

resource__shader *get_shader_resource(resource_token token)
{
    resource__shader *result = token.system->shaders.data() + token.index;
    return result;
}

resource__texture *get_texture_resource(resource_token token)
{
    resource__texture *result = token.system->textures.data() + token.index;
    return result;
}

resource_token create_mesh_resource(resource_system *system, memory_block vbo, memory_block ibo, gfx::vertex_buffer_layout vbl)
{
    resource_token token = {};

    if (system->meshes.size() < system->meshes.capacity())
    {
        resource__mesh resource;
        resource.vbo = ALLOCATE_COPY(system->heap, vbo);
        resource.ibo = ALLOCATE_COPY(system->heap, ibo);
        resource.vbl = vbl;
        resource.render_data = memory__empty_block();

        token.system = system;
        token.type = RESOURCE_MESH;
        token.index = (uint32) system->meshes.size();
        system->meshes.push(resource);
    }
    return token;
}

resource_token create_shader_resource(resource_system *system, string_id name)
{
    resource_token token = {};

    if (system->shaders.size() < system->shaders.capacity())
    {
        resource__shader resource;
        resource.name = name;
        resource.render_data = memory__empty_block();

        token.system = system;
        token.type = RESOURCE_SHADER;
        token.index = (uint32) system->shaders.size();
        system->shaders.push(resource);
    }
    return token;
}

resource_token create_texture_resource(resource_system *system, bitmap texture)
{
    resource_token token = {};

    if (system->textures.size() < system->textures.capacity())
    {
        resource__texture resource;
        resource.texture = texture;
        resource.render_data = memory__empty_block();

        token.system = system;
        token.type = RESOURCE_TEXTURE;
        token.index = (uint32) system->textures.size();
        system->textures.push(resource);
    }
    return token;
}

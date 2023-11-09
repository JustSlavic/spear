#include "renderer.hpp"
#include <gfx/gl.hpp>
#include <rs/resource_system.hpp>

#include <math/integer.hpp>
#include <g301.hpp>
#include <math/rectangle2.hpp>


namespace gfx {
namespace gl {

GLOBAL char const *vs_source = R"GLSL(
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

out vec4 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_color;

void main()
{
    vec4 p = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    fragment_color = vec4(vertex_color, 1.0) + u_color;
    gl_Position = p;
}
)GLSL";


GLOBAL char const *fs_source = R"GLSL(
#version 400

in vec4 fragment_color;

out vec4 result_color;

void main()
{
    result_color = fragment_color;
}
)GLSL";


GLOBAL char const *fs_circle_source = R"GLSL(
#version 400
#define lerp mix

in vec4 fragment_color;
in vec2 fragment_position;

out vec4 result_color;

void main()
{
    if (dot(fragment_position, fragment_position) < 1.0)
        result_color = fragment_color;
    else
        result_color = vec4(0, 0, 0, 0);
    // result_color = lerp(fragment_color, vec4(1, 1, 1, 1), dot(fragment_position, fragment_position));
}

)GLSL";

GLOBAL char const *vs_textured_source = R"GLSL(
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 uv_coordinates;

out vec2 position;
out vec2 uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    vec4 p = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    position = vertex_position.xy;
    uv = uv_coordinates;
    gl_Position = p;
}
)GLSL";

GLOBAL char const *fs_textured_source = R"GLSL(
#version 400

in vec2 position;
in vec2 uv;

out vec4 result_color;

uniform sampler2D u_texture0;

void main()
{
    vec4 texture_color = texture(u_texture0, uv);
    result_color = texture_color;
}
)GLSL";

GLOBAL char const *vs_frame_source = R"GLSL(
#version 400

#define BORDER_WIDTH  (1.0 / 16.0) * 0.1
#define BORDER_HEIGHT (1.0 /  9.0) * 0.1

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_add_sign;

out vec4 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_color;

void main()
{
    vec4 p = u_projection * u_view * u_model * vec4(vertex_position, 0.0, 1.0);
    p = p + vec4(vertex_add_sign.x * BORDER_WIDTH, vertex_add_sign.y * BORDER_HEIGHT, 0, 0);
    // Accounting to the fact that UI is Y-top-down, but I draw AABB in the Y-down-top
    p.y *= -1.0;

    fragment_color = u_color;
    gl_Position = p;
}
)GLSL";


GLOBAL char const *fs_frame_source = R"GLSL(
#version 400

in vec4 fragment_color;
out vec4 result_color;

void main()
{
    result_color = fragment_color;
}
)GLSL";



matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};

    result._11 = 2.0f * n / w;
    result._22 = 2.0f * n / h;
    result._33 = -(f + n) / (f - n);
    result._34 = -2.0f * f * n / (f - n);
    result._43 = -1.0f;

    return result;
}

matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    //     w/2
    //   +-----+
    //   |    /
    //   |   /
    // n |  /
    //   | / angle = fov/2
    //   |/  tg(fov / 2) = (w/2) / n
    //   +   => 2n / w = 1 / tg(fov / 2)

    float32 tf2 = (1.0f / math::tg(0.5f * fov));

    matrix4 result = {};

    result._11 = tf2;
    result._22 = tf2 * aspect_ratio;
    result._33 = -(f + n) / (f - n);
    result._34 = -2.0f * f * n / (f - n);
    result._43 = -1.0f;

    return result;
}

matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    matrix4 result = {};

    result._11 = 2.0f / w;
    result._22 = 2.0f / h;
    result._33 = -2.0f / (f - n);
    result._34 = -(f + n) / (f - n);
    result._44 = 1.0f;

    return result;
}

matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    matrix4 result;

    result._11 = 1.0f;
    result._22 = 1.0f * aspect_ratio;
    result._33 = -2.0f / (f - n);
    result._34 = -(f + n) / (f - n);
    result._44 = 1.0f;

    return result;
}

void setup_projection_matrix(render_command *cmd)
{

}

void setup_camera(render_command *cmd)
{

}

struct render_mesh_data
{
    uint32 vertex_buffer_id;
    uint32 index_buffer_id;
    uint32 vertex_array_id;
};


struct render_shader_data
{
    shader program;
};

struct render_texture_data
{
    uint32 texture_id;
    bool32 is_top_down;
    float32 aspect_ratio;
};

void load_mesh(execution_context *context, resource__mesh *mesh)
{
    uint32 vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, mesh->vbo.size, mesh->vbo.memory, GL_STATIC_DRAW);
    }

    uint32 index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo.size, mesh->ibo.memory, GL_STATIC_DRAW);
    }

    uint32 vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        uint32 stride = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh->vbl.element_count; attrib_index++)
        {
            stride += (mesh->vbl.elements[attrib_index].count * sizeof(float32));
        }

        usize offset = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh->vbl.element_count; attrib_index++)
        {
            uint32 count = mesh->vbl.elements[attrib_index].count;
            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                stride,            // Stride
                (void *) offset);  // Offset

            offset += (count * sizeof(float32));
        }
    }

    if (mesh->render_data.memory == NULL)
    {
        mesh->render_data = ALLOCATE_TYPE_IN_BLOCK(context->renderer_allocator, render_mesh_data);
    }

    auto *data = (render_mesh_data *) mesh->render_data.memory;
    data->vertex_buffer_id = vertex_buffer_id;
    data->index_buffer_id = index_buffer_id;
    data->vertex_array_id = vertex_array_id;
}

void load_shader(execution_context *context, resource__shader *shader)
{
    uint32 vs = 0;
    uint32 fs = 0;
    if (shader->name == STRID("rectangle.shader"))
    {
        // @todo: load this from file
        vs = compile_shader(vs_source, shader::vertex);
        fs = compile_shader(fs_source, shader::fragment);
    }
    if (shader->name == STRID("circle.shader"))
    {
        // @todo: load this from file
        vs = compile_shader(vs_source, shader::vertex);
        fs = compile_shader(fs_circle_source, shader::fragment);
    }
    if (shader->name == STRID("rectangle_uv.shader"))
    {
        vs = compile_shader(vs_textured_source, shader::vertex);
        fs = compile_shader(fs_textured_source, shader::fragment);
    }
    if (shader->name == STRID("frame.shader"))
    {
        vs = compile_shader(vs_frame_source, shader::vertex);
        fs = compile_shader(fs_frame_source, shader::fragment);
    }

    auto program = link_shader(vs, fs);

    if (shader->render_data.memory == NULL)
        shader->render_data = ALLOCATE_TYPE_IN_BLOCK(context->renderer_allocator, render_shader_data);

    auto *data = (render_shader_data *) shader->render_data.memory;
    data->program = program;
}

void load_texture(execution_context *context, resource__texture *texture)
{
    if (texture->render_data.memory == NULL)
    {
        texture->render_data = ALLOCATE_TYPE_IN_BLOCK(context->renderer_allocator, render_texture_data);
    }

    auto *data = (render_texture_data *) texture->render_data.memory;
    data->texture_id = create_texture(texture->texture);
    data->is_top_down = texture->texture.top_down;
    data->aspect_ratio = (float32) texture->texture.width / (float32) texture->texture.height;
}

void draw_indexed_triangles(resource__mesh *mesh,
                            resource__shader *shader,
                            matrix4 model,
                            matrix4 view,
                            matrix4 projection,
                            vector4 color)
{
    auto *mesh_render_data = (render_mesh_data *) mesh->render_data.memory;
    auto *shader_render_data = (render_shader_data *) shader->render_data.memory;

    use_shader(shader_render_data->program);
    uniform(shader_render_data->program, "u_model", model);
    uniform(shader_render_data->program, "u_view", view);
    uniform(shader_render_data->program, "u_projection", projection);
    uniform(shader_render_data->program, "u_color", color);

    auto a = V4(1, 1, 0, 1) * model * view * projection;
    auto b = projection * view * model * V4(1, 1, 0, 1);

    glBindVertexArray(mesh_render_data->vertex_array_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_render_data->index_buffer_id);
    glDrawElements(GL_TRIANGLES, truncate_to_int32(mesh->ibo.size) / sizeof(int32), GL_UNSIGNED_INT, NULL);
}

void draw_indexed_triangles(resource__mesh *mesh,
                            resource__shader *shader,
                            resource__texture *texture,
                            matrix4 model,
                            matrix4 view,
                            matrix4 projection)
{
    auto *mesh_data = (render_mesh_data *) mesh->render_data.memory;
    auto *shader_data = (render_shader_data *) shader->render_data.memory;
    auto *texture_data = (render_texture_data *) texture->render_data.memory;

    use_shader(shader_data->program);
    use_texture(texture_data->texture_id, 0);

    model._22 *= 1.0f/texture_data->aspect_ratio;
    // if (!texture_data->is_top_down)
    // {
    //     // @todo: aspect ratio have to have the effect different than that for sure
    // }
    uniform(shader_data->program, "u_model", model);
    uniform(shader_data->program, "u_view", view);
    uniform(shader_data->program, "u_projection", projection);
    uniform(shader_data->program, "u_texture0", 0);

    // use_texture(texture_data->texture_id, 0);
    glBindVertexArray(mesh_data->vertex_array_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data->index_buffer_id);
    glDrawElements(GL_TRIANGLES, truncate_to_int32(mesh->ibo.size) / sizeof(int32), GL_UNSIGNED_INT, NULL);
}


} // namespace gfx
} // namespace gl

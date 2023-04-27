#include "renderer.hpp"
#include <gfx/gl.hpp>
#include <rs/resource.hpp>

#include <math/integer.hpp>
#include <math/vector4.hpp>
#include <math/rectangle2.hpp>


namespace gfx::gl {

GLOBAL char const *vs_source = R"GLSL(
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

out vec4 fragment_color;
out vec2 fragment_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_color;

void main()
{
    vec4 p = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    fragment_color = vec4(vertex_color, 1.0) + u_color;
    fragment_position = vertex_position.xy;
    gl_Position = p;
}
)GLSL";


GLOBAL char const *fs_source = R"GLSL(
#version 400

in vec4 fragment_color;
in vec2 fragment_screen_position;

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


math::matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    math::matrix4 result = {};

    result._11 = 2.0f * n / w;
    result._22 = 2.0f * n / h;
    result._33 = -(f + n) / (f - n);
    result._34 = -1.0f;
    result._43 = -2.0f * f * n / (f - n);

    return result;
}

math::matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
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

    math::matrix4 result = {};

    result._11 = tf2;
    result._22 = tf2 * aspect_ratio;
    result._33 = -(f + n) / (f - n);
    result._34 = -1.0f;
    result._43 = -2.0f * f * n / (f - n);

    return result;
}

math::matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    math::matrix4 result = {};

    result._11 = 2.0f / w;
    result._22 = 2.0f / h;
    result._33 = -2.0f / (f - n);
    result._43 = -(f + n) / (f - n);
    result._44 = 1.0f;

    return result;
}

math::matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    math::matrix4 result;

    result._11 = 1.0f;
    result._22 = 1.0f * aspect_ratio;
    result._33 = -2.0f / (f - n);
    result._43 = -(f + n) / (f - n);
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


void load_mesh(execution_context *context, rs::resource *resource)
{
    uint32 vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, resource->mesh.vbo.size, resource->mesh.vbo.memory, GL_STATIC_DRAW);
    }

    uint32 index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, resource->mesh.ibo.size, resource->mesh.ibo.memory, GL_STATIC_DRAW);
    }

    uint32 vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        uint32 stride = 0;
        for (uint32 attrib_index = 0; attrib_index < resource->mesh.vbl.element_count; attrib_index++)
        {
            stride += (resource->mesh.vbl.elements[attrib_index].count * sizeof(float32));
        }

        usize offset = 0;
        for (uint32 attrib_index = 0; attrib_index < resource->mesh.vbl.element_count; attrib_index++)
        {
            uint32 count = resource->mesh.vbl.elements[attrib_index].count;
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

    if (resource->render_data == NULL)
    {
        resource->render_data = ALLOCATE(&context->renderer_allocator, render_mesh_data);
    }

    auto *data = (render_mesh_data *) resource->render_data;
    data->vertex_buffer_id = vertex_buffer_id;
    data->index_buffer_id = index_buffer_id;
    data->vertex_array_id = vertex_array_id;
}

void load_shader(execution_context *context, rs::resource *resource)
{
    uint32 vs = 0;
    uint32 fs = 0;
    if (resource->shader.name == STRID("rectangle.shader"))
    {
        // @todo: load this from file
        vs = compile_shader(vs_source, shader::vertex);
        fs = compile_shader(fs_source, shader::fragment);
    }
    if (resource->shader.name == STRID("circle.shader"))
    {
        // @todo: load this from file
        vs = compile_shader(vs_source, shader::vertex);
        fs = compile_shader(fs_circle_source, shader::fragment);
    }

    auto program = link_shader(vs, fs);

    if (resource->render_data == NULL)
    {
        resource->render_data = ALLOCATE(&context->renderer_allocator, render_shader_data);
    }

    auto *data = (render_shader_data *) resource->render_data;
    data->program = program;
}

void draw_indexed_triangles(rs::resource *mesh, rs::resource *shader, math::matrix4 model, math::matrix4 view, math::matrix4 projection, math::vector4 color)
{
    auto *mesh_render_data = (render_mesh_data *) mesh->render_data;
    auto *shader_render_data = (render_shader_data *) shader->render_data;

    use_shader(shader_render_data->program);
    uniform(shader_render_data->program, "u_model", model);
    uniform(shader_render_data->program, "u_view", view);
    uniform(shader_render_data->program, "u_projection", projection);
    uniform(shader_render_data->program, "u_color", color);

    glBindVertexArray(mesh_render_data->vertex_array_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_render_data->index_buffer_id);
    glDrawElements(GL_TRIANGLES, usize_to_int32(mesh->mesh.ibo.size) / sizeof(int32), GL_UNSIGNED_INT, NULL);
}


} // namespace gfx::gl

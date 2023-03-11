#include "renderer.hpp"
#include <gfx/opengl/gl.hpp>

#include <math/integer.hpp>
#include <math/vector4.hpp>
#include <math/rectangle2.hpp>


namespace gfx
{

namespace gl
{

GLOBAL char const *vs_source = R"GLSL(
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

out vec4 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    vec4 p = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    fragment_color = vec4(vertex_color, 1.0);
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

struct render_with_indices
{
    uint32 vbo;
    uint32 ibo;
    uint32 ibo_size;
    uint32 vao;
};

render_with_indices opengl_rectangle(math::rectangle2 rect, math::vector4 color)
{
    struct vertex
    {
        math::vector3 position;
        math::vector3 color;
    };

    vertex vertices[] =
    {
        { math::make_vector3(rect.min.x, rect.min.y, 0), color.rgb }, // 0 bottom left
        { math::make_vector3(rect.max.x, rect.min.y, 0), color.rgb }, // 1 bottom right
        { math::make_vector3(rect.max.x, rect.max.y, 0), color.rgb }, // 2 top right
        { math::make_vector3(rect.min.x, rect.max.y, 0), color.rgb }, // 3 top left
    };

    uint32 vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    uint32 indices[] = {
        0, 1, 2,  // first triangle
        2, 3, 0,  // second triangle
    };

    uint32 index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    uint32 vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        uint32 attrib_index = 0;
        uint64 offset = 0;
        {
            uint32 count = 3; // Because it's vector3

            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                sizeof(vertex),    // Stride
                (void *) offset);  // Offset

            attrib_index += 1;
            offset += (count * sizeof(float32));
        }
        {
            uint32 count = 3; // Because it's color32

            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                sizeof(vertex),    // Stride
                (void *) offset);  // Offset

            attrib_index += 1;
            offset += (count * sizeof(float32));
        }
    }

    render_with_indices result;
    result.vbo = vertex_buffer_id;
    result.ibo = index_buffer_id;
    result.ibo_size = sizeof(indices);
    result.vao = vertex_array_id;

    return result;
}

void draw_rectangle(render_command *cmd, math::matrix4 view, math::matrix4 projection)
{
    PERSIST auto plane_vs = compile_shader(vs_source, shader::vertex);
    PERSIST auto plane_fs = compile_shader(fs_source, shader::fragment);
    PERSIST auto shader = link_shader(plane_vs, plane_fs);

    auto buffers = opengl_rectangle(cmd->draw_rectangle.rect, {1, 1, 1, 1});
    use_shader(shader);
    uniform(shader, "u_model", math::matrix4::identity());
    uniform(shader, "u_view", view);
    uniform(shader, "u_projection", projection);

    glBindVertexArray(buffers.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ibo);
    glDrawElements(GL_TRIANGLES, buffers.ibo_size, GL_UNSIGNED_INT, NULL);
}

struct render_mesh_data
{
    uint32 vertex_buffer_id;
    uint32 index_buffer_id;
    uint32 vertex_array_id;
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

void draw_indexed_triangles(rs::resource *resource, math::matrix4 model, math::matrix4 view, math::matrix4 projection)
{
    PERSIST auto plane_vs = compile_shader(vs_source, shader::vertex);
    PERSIST auto plane_fs = compile_shader(fs_source, shader::fragment);
    PERSIST auto shader = link_shader(plane_vs, plane_fs);

    use_shader(shader);
    uniform(shader, "u_model", model);
    uniform(shader, "u_view", view);
    uniform(shader, "u_projection", projection);

    auto *render_data = (render_mesh_data *) resource->render_data;

    glBindVertexArray(render_data->vertex_array_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data->index_buffer_id);
    glDrawElements(GL_TRIANGLES, truncate_int32(resource->mesh.ibo.size), GL_UNSIGNED_INT, NULL);
}


} // namespace gl

} // namespace gfx

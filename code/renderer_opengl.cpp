#include <renderer.hpp>
#include <opengl/gl.hpp>

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

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    vec4 p = u_projection * u_view * vec4(vertex_position, 1.0);
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


void setup_projection_matrix(render_command *cmd)
{

}

void setup_camera(render_command *cmd)
{

}

struct render_with_indices
{
    uint32_t vbo;
    uint32_t ibo;
    uint32_t ibo_size;
    uint32_t vao;
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

    uint32_t vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    uint32_t indices[] = {
        0, 1, 2,  // first triangle
        2, 3, 0,  // second triangle
    };

    uint32_t index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    uint32_t vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        uint32_t attrib_index = 0;
        uint64_t offset = 0;
        {
            uint32_t count = 3; // Because it's vector3

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
            uint32_t count = 3; // Because it's color32

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

    auto buffers = opengl_rectangle(cmd->rect, {1, 1, 1, 1});
    use_shader(shader);
    uniform(shader, "u_view", view);
    uniform(shader, "u_projection", projection);

    glBindVertexArray(buffers.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ibo);
    glDrawElements(GL_TRIANGLES, buffers.ibo_size, GL_UNSIGNED_INT, NULL);
}

} // namespace gl

} // namespace gfx

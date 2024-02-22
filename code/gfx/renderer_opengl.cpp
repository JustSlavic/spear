#include "renderer_opengl.hpp"

#include <integer.h>

#include "gl.hpp"

#include "static_shaders.cpp"


namespace gl
{

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

    float32 tf2 = (1.0f / tanf(0.5f * fov));

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

// ======================================================================

struct mesh_render_data
{
    uint32 vbo_id;
    uint32 ibo_id;
    uint32 vao_id;

    uint32 count;
};

mesh_render_data *load_mesh(rs::mesh *mesh)
{
    uint32 vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, mesh->vbo.size, mesh->vbo.data, GL_STATIC_DRAW);
    }

    uint32 index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo.size, mesh->ibo.data, GL_STATIC_DRAW);
    }

    uint32 vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        uint32 stride = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh->vbl.count; attrib_index++)
        {
            stride += (mesh->vbl.elements[attrib_index].count * sizeof(float32));
        }

        usize offset = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh->vbl.count; attrib_index++)
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

    if (mesh->render_data == NULL)
    {
        mesh->render_data = mallocator().allocate<mesh_render_data>();
    }

    auto mrd = (mesh_render_data *) mesh->render_data;
    mrd->vbo_id = vertex_buffer_id;
    mrd->ibo_id = index_buffer_id;
    mrd->vao_id = vertex_array_id;
    mrd->count  = truncate_to_int32(mesh->ibo.size / sizeof(int32));

    return mrd;
}

struct shader_render_data
{
    uint32 program_id;
    uint32 vs_id;
    uint32 fs_id;
};

shader_render_data *load_shader(rs::shader *s)
{
    uint32 vs = 0;
    uint32 fs = 0;

    // For now use only one shader
    {
        vs = gl::compile_shader(memory_buffer::from(vs_single_color), gl::shader::vertex);
        fs = gl::compile_shader(memory_buffer::from(fs_pass_color), gl::shader::fragment);
    }

    auto program = gl::link_shader(vs, fs);

    if (s->render_data == NULL)
    {
        s->render_data = mallocator().allocate<shader_render_data>();
    }

    auto *rsd = (shader_render_data *) s->render_data;
    rsd->program_id = program;
    rsd->vs_id = vs;
    rsd->fs_id = fs;

    return rsd;
}

void draw_indexed_triangles(matrix4 model, matrix4 view, matrix4 proj, mesh_render_data *mesh_rd, shader_render_data *shader_rd, vector4 color)
{
    shader s = { shader_rd->program_id, shader_rd->vs_id, shader_rd->fs_id };
    use_shader(s);

    s.uniform("u_model", model);
    s.uniform("u_view", view);
    s.uniform("u_projection", proj);
    s.uniform("u_color", color);

    auto m = proj * view * model * V4(1, 1, 0, 1);

    glBindVertexArray(mesh_rd->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_rd->ibo_id);
    glDrawElements(GL_TRIANGLES, mesh_rd->count, GL_UNSIGNED_INT, NULL);
}

void render_mesh_single_color(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh_token, rs::token shader_token, vector4 color)
{
    ASSERT(mesh_token.kind == rs::resource_kind::mesh);

    if (mesh_token.kind != rs::resource_kind::mesh) { return; }
    if (shader_token.kind != rs::resource_kind::shader) { return; }

    auto mesh = ctx->rs->get_mesh(mesh_token);
    auto mesh_rd = mesh->render_data ? (mesh_render_data *) mesh->render_data : gl::load_mesh(mesh);

    auto shader = ctx->rs->get_shader(shader_token);
    auto shader_rd = shader->render_data ? (shader_render_data *) shader->render_data : gl::load_shader(shader);

    draw_indexed_triangles(model, view, proj, mesh_rd, shader_rd, color);
}

} // namespace gl

#include "renderer_opengl.hpp"
#include <math/integer.h>
#include "gl.hpp"
// #include "static_shaders.cpp"
#include <memory_bucket.hpp>


namespace gl
{

struct driver
{
    gfx::api m_api;
    uint32 *debug_layer_ids[8];
    uint8 data[64];
};

void debug_layer_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam)
{
    printf("OpenGL Debug Layer: %.*s\n", (int) length, message);
}

void enable_debug_layer(void *d)
{
#if 0
    auto *opengl_driver = (driver *) d;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, ARRAY_COUNT(driver::debug_layer_ids), (uint32 *) opengl_driver->debug_layer_ids, true);
    glDebugMessageCallback(debug_layer_callback, NULL);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
}

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
    matrix4 result = {};

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

mesh_render_data *load_mesh(context *ctx, rs::mesh *mesh)
{
    uint32 vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        GL_CHECK_ERRORS();
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        GL_CHECK_ERRORS();
        glBufferData(GL_ARRAY_BUFFER, mesh->vbo.size, mesh->vbo.data, GL_STATIC_DRAW);
        GL_CHECK_ERRORS();
    }

    uint32 index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        GL_CHECK_ERRORS();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        GL_CHECK_ERRORS();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo.size, mesh->ibo.data, GL_STATIC_DRAW);
        GL_CHECK_ERRORS();
    }

    uint32 vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        GL_CHECK_ERRORS();
        glBindVertexArray(vertex_array_id);
        GL_CHECK_ERRORS();
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        GL_CHECK_ERRORS();

        uint32 stride = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh->vbl.size; attrib_index++)
        {
            stride += (mesh->vbl.elements[attrib_index].count * sizeof(float32));
        }

        usize offset = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh->vbl.size; attrib_index++)
        {
            uint32 count = mesh->vbl.elements[attrib_index].count;
            glEnableVertexAttribArray(attrib_index);
            GL_CHECK_ERRORS();
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                stride,            // Stride
                (void *) offset);  // Offset
            GL_CHECK_ERRORS();

            offset += (count * sizeof(float32));
        }
    }

    if (mesh->render_data == NULL)
    {
        mesh->render_data = ctx->renderer_allocator.allocate<mesh_render_data>();
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

shader_render_data *load_shader(context *ctx, rs::shader *s)
{
    uint32 vs = 0;
    uint32 fs = 0;

    if (s->name == string_id::from(ctx->strids, "SHADER_SINGLE_COLOR"))
    {
        // vs = compile_shader(memory_buffer::from(vs_single_color), shader::vertex);
        // fs = compile_shader(memory_buffer::from(fs_pass_color), shader::fragment);
    }
    else if (s->name == string_id::from(ctx->strids, "SHADER_DRAW_TEXTURE"))
    {
        // vs = compile_shader(memory_buffer::from(vs_uv_coords), shader::vertex);
        // fs = compile_shader(memory_buffer::from(fs_apply_texture), shader::fragment);
    }
    else if (s->name == string_id::from(ctx->strids, "SHADER_DRAW_TEXT"))
    {
        // vs = compile_shader(memory_buffer::from(vs_text_shader), shader::vertex);
        // fs = compile_shader(memory_buffer::from(fs_text_shader), shader::fragment);
    }
    else if (s->name == string_id::from(ctx->strids, "SHADER_DRAW_GROUND"))
    {
        // vs = compile_shader(memory_buffer::from(vs_shaded_cube), shader::vertex);
        // fs = compile_shader(memory_buffer::from(fs_pass_color), shader::fragment);
    }
    else
    {
        ASSERT_FAIL();
    }

    if (vs == 0 || fs == 0) return NULL;

    auto program = gl::link_shader(vs, fs);

    if (s->render_data == NULL)
    {
        s->render_data = ctx->renderer_allocator.allocate<shader_render_data>();
    }

    auto *rsd = (shader_render_data *) s->render_data;
    rsd->program_id = program;
    rsd->vs_id = vs;
    rsd->fs_id = fs;

    return rsd;
}

struct texture_render_data
{
    uint32 texture_id;
};

texture_render_data *load_texture(context *ctx, rs::texture *tx)
{
    uint32 tex_id = gl::load_texture(tx->data);

    if (tx->render_data == NULL)
    {
        tx->render_data = ctx->renderer_allocator.allocate<texture_render_data>();
    }

    auto *trd = (texture_render_data *) tx->render_data;
    trd->texture_id = tex_id;

    return trd;
}

void draw_indexed_triangles_color(matrix4 model, matrix4 view, matrix4 proj, mesh_render_data *mesh_rd, shader_render_data *shader_rd, vector4 color)
{
    shader s = { shader_rd->program_id, shader_rd->vs_id, shader_rd->fs_id };
    use_shader(s);

    s.uniform("u_model", model);
    s.uniform("u_view", view);
    s.uniform("u_projection", proj);
    s.uniform("u_color", color);

    glBindVertexArray(mesh_rd->vao_id);
    GL_CHECK_ERRORS();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_rd->ibo_id);
    GL_CHECK_ERRORS();
    glDrawElements(GL_TRIANGLES, mesh_rd->count, GL_UNSIGNED_INT, NULL);
    GL_CHECK_ERRORS();
}

void draw_indexed_triangles_texture(matrix4 model, matrix4 view, matrix4 proj, mesh_render_data *mesh_rd, shader_render_data *shader_rd, texture_render_data *texture_rd)
{
    shader s = { shader_rd->program_id, shader_rd->vs_id, shader_rd->fs_id };
    use_shader(s);
    use_texture(texture_rd->texture_id, 0);

    s.uniform("u_model", model);
    s.uniform("u_view", view);
    s.uniform("u_projection", proj);

    glBindVertexArray(mesh_rd->vao_id);
    GL_CHECK_ERRORS();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_rd->ibo_id);
    GL_CHECK_ERRORS();
    glDrawElements(GL_TRIANGLES, mesh_rd->count, GL_UNSIGNED_INT, NULL);
    GL_CHECK_ERRORS();
}

void render_mesh_single_color(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh_token, rs::token shader_token, vector4 color)
{
    ASSERT(mesh_token.kind == rs::resource_kind::mesh);
    ASSERT(shader_token.kind == rs::resource_kind::shader);

    if (mesh_token.kind != rs::resource_kind::mesh) { return; }
    if (shader_token.kind != rs::resource_kind::shader) { return; }

    auto mesh = ctx->rs->get_mesh(mesh_token);
    auto mesh_rd = mesh->render_data ? (mesh_render_data *) mesh->render_data : gl::load_mesh(ctx, mesh);

    auto shader = ctx->rs->get_shader(shader_token);
    auto shader_rd = shader->render_data ? (shader_render_data *) shader->render_data : gl::load_shader(ctx, shader);

    draw_indexed_triangles_color(model, view, proj, mesh_rd, shader_rd, color);
}

void render_mesh_texture(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh_token, rs::token shader_token, rs::token texture_token)
{
    ASSERT(mesh_token.kind == rs::resource_kind::mesh);
    ASSERT(shader_token.kind == rs::resource_kind::shader);
    ASSERT(texture_token.kind == rs::resource_kind::texture);

    if (mesh_token.kind != rs::resource_kind::mesh) { return; }
    if (shader_token.kind != rs::resource_kind::shader) { return; }
    if (texture_token.kind != rs::resource_kind::texture) { return; }

    auto mesh = ctx->rs->get_mesh(mesh_token);
    auto mesh_rd = mesh->render_data ? (mesh_render_data *) mesh->render_data : gl::load_mesh(ctx, mesh);

    auto shader = ctx->rs->get_shader(shader_token);
    auto shader_rd = shader->render_data ? (shader_render_data *) shader->render_data : gl::load_shader(ctx, shader);

    auto texture = ctx->rs->get_texture(texture_token);
    auto texture_rd = texture->render_data ? (texture_render_data *) texture->render_data : gl::load_texture(ctx, texture);

    draw_indexed_triangles_texture(model, view, proj, mesh_rd, shader_rd, texture_rd);
}

#include <gen/font.hpp>

void render_text(context *ctx, matrix4 proj, rs::token mesh_token, rs::token shader_token, rs::token texture_token, string_view text, vector4 color)
{
    // char buffer[1024];
    // snprintf(buffer, sizeof(buffer), "Render text: %.*s\n", (int) text.size, text.data);

    // OutputDebugStringA(buffer);

    if (mesh_token.kind != rs::resource_kind::mesh) { return; }
    if (shader_token.kind != rs::resource_kind::shader) { return; }

    auto mesh = ctx->rs->get_mesh(mesh_token);
    auto mesh_rd = mesh->render_data ? (mesh_render_data *) mesh->render_data : gl::load_mesh(ctx, mesh);

    auto shader_ = ctx->rs->get_shader(shader_token);
    auto shader_rd = shader_->render_data ? (shader_render_data *) shader_->render_data : gl::load_shader(ctx, shader_);

    auto texture = ctx->rs->get_texture(texture_token);
    auto texture_rd = texture->render_data ? (texture_render_data *) texture->render_data : gl::load_texture(ctx, texture);

    auto model = matrix4::identity();
    auto view = matrix4::identity();

    shader s = { shader_rd->program_id, shader_rd->vs_id, shader_rd->fs_id };
    use_shader(s);
    s.uniform("u_projection", proj);
    s.uniform("u_color", color);

    use_texture(texture_rd->texture_id, 0);

    int32 posx = 0;
    int32 posy = 0;

    auto vbo_data_buffer = ctx->temporary_allocator.allocate_buffer(sizeof(float32) * 24 * text.size);
    auto vbo_bucket = memory_bucket::from(vbo_data_buffer);
    uint32 count = 0;

    for (int char_index = 0; char_index < text.size; char_index++)
    {
        char c = text.data[char_index];
        for (int i = 0; i < font_Arial.characterCount; i++)
        {
            auto glyph = font_Arial.characters[i];
            if (glyph.codePoint == c)
            {
                float32 px = (float32) posx - glyph.originX;
                float32 py = (float32) posy - glyph.originY;
                float32 w  = (float32) glyph.width;
                float32 h  = (float32) glyph.height;

                float32 uv_x = (float32) glyph.x / font_Arial.width;
                float32 uv_y = (float32) glyph.y / font_Arial.height;
                float32 uv_x1 = (float32) (glyph.x + glyph.width) / font_Arial.width;
                float32 uv_y1 = (float32) (glyph.y + glyph.height) / font_Arial.height;

                float32 vbo_data[] = {
                     px,     py,       uv_x,  uv_y,
                     px + w, py,       uv_x1, uv_y,
                     px    , py + h,   uv_x,  uv_y1,

                     px + w, py,       uv_x1, uv_y,
                     px + w, py + h,   uv_x1, uv_y1,
                     px,     py + h,   uv_x,  uv_y1,
                };
                vbo_bucket.append(vbo_data, sizeof(vbo_data));

                posx += glyph.width;
                count += 6;
            }
        }
    }

    glBindVertexArray(mesh_rd->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_rd->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vbo_bucket.used, vbo_bucket.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, count);
}


framebuffer *create_framebuffer(context *ctx)
{
    framebuffer *result = NULL;

    uint32 texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->viewport.width, ctx->viewport.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    GL_CHECK_ERRORS();

    uint32 render_buffer_id = 0;
    glGenRenderbuffers(1, &render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ctx->viewport.width, ctx->viewport.height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    GL_CHECK_ERRORS();

    uint32 id = 0;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECK_ERRORS();

    auto completenes_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (completenes_status)
    {
    case GL_FRAMEBUFFER_UNDEFINED: printf("Specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("Any of the framebuffer attachment points are framebuffer incomplete.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("The framebuffer does not have at least one image attached to it.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: printf("The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: printf("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.\n"); break;
    case GL_FRAMEBUFFER_UNSUPPORTED: printf("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: printf("The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.\n");
        printf("    OR\n");
        printf("The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: printf("Any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.\n"); break;
    case GL_FRAMEBUFFER_COMPLETE: printf("Framebuffer created correctly\n"); break;
    }
    if(completenes_status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Error: Framebuffer is not complete!\n");
        return result;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    result = ctx->renderer_allocator.allocate<framebuffer>();
    result->framebuffer_id = id;
    result->color_texture_id = texture_id;
    result->depth_stencil_id = render_buffer_id;
    return result;
}

void destroy_framebuffer(framebuffer *fb)
{
    glDeleteFramebuffers(1, &fb->framebuffer_id);
    // ctx->renderer_allocator.deallocate(fb);
}

void use_framebuffer(framebuffer *fb)
{
    uint32 id = fb ? fb->framebuffer_id : 0;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    GL_CHECK_ERRORS();
}




} // namespace gl

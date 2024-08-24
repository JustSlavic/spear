#ifndef COMMON_GRAPHICS_HPP
#define COMMON_GRAPHICS_HPP

#include <base.h>
#include <math/matrix4.hpp>
#include <math/vector3.hpp>
#include <console.hpp>
#include <image/bitmap.hpp>


matrix4 make_lookat_matrix(vector3 eye, vector3 at, vector3 up)
{
    vector3 f = normalized(at - eye);
    vector3 s = normalized(cross(f, up));
    vector3 u = cross(s, f);

    matrix4 result = {};
    result._1 = V4(s, -inner(s, eye));
    result._2 = V4(u, -inner(u, eye));
    result._3 = V4(-f, inner(f, eye));
    result._4 = V4(0, 0, 0, 1);

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

// @note: for now always floats
struct vertex_buffer_layout_element
{
    uint32 count;
    // @todo: other types of elements other than floats
};

struct vertex_buffer_layout
{
    uint32 size;
    vertex_buffer_layout_element elements[8];

    static vertex_buffer_layout make()
    {
        vertex_buffer_layout result = {};
        return result;
    }

    template <typename T>
    void push(uint32 count)
    {
        ASSERT(count < ARRAY_COUNT(elements));
        elements[size++].count = count;
    }
};

struct cpu_mesh
{
    memory_buffer vbo;
    memory_buffer ibo;
    vertex_buffer_layout vbl;
};

struct gpu_mesh
{
    uint32 vbo;
    uint32 ibo;
    uint32 vao;
    uint32 count;
};

struct texture
{
    uint32 id;
};

struct gpu_text
{
    char const *cstr;
    texture font_texture;
    gpu_mesh mesh;
};

struct shader
{
    uint32 id;
    uint32 vs_id;
    uint32 fs_id;

    enum shader_type
    {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
    };

    void uniform(char const *name, int32 n)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform1i(location, n);
    }

    void uniform(char const *name, float f)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform1f(location, f);
    }

    void uniform(char const *name, vector2 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform2f(location, v.x, v.y);
    }

    void uniform(char const *name, vector3 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform3f(location, v.x, v.y, v.z);
    }

    void uniform(char const *name, vector4 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }

    void uniform(char const *name, matrix4 const& m)
    {
        auto location = glGetUniformLocation(id, name);
        glUniformMatrix4fv(location, 1, GL_TRUE, get_data(m));
    }
};

struct framebuffer
{
    uint32 framebuffer_id;
    uint32 color_texture_id;
    uint32 depth_stencil_id;
};

cpu_mesh make_square()
{
    static float32 vbo_data[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
    };

    static uint32 ibo_data[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto ibo = memory_buffer::from(ibo_data, sizeof(ibo_data));

    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

cpu_mesh make_cube()
{
    static float32 vbo_data[] = {
        // bottom square
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        // top square
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
    };

    static uint32 ibo_data[] = {
        0, 1, 2,
        2, 3, 0,

        0, 3, 7,
        7, 4, 0,

        1, 5, 6,
        6, 2, 1,

        3, 2, 6,
        6, 7, 3,

        1, 0, 4,
        4, 5, 1,

        5, 4, 7,
        7, 6, 5,
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto ibo = memory_buffer::from(ibo_data, sizeof(ibo_data));

    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

cpu_mesh make_square_uv()
{
    static float32 vbo_data[] = {
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,
    };
    static uint32 ibo_data[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto ibo = memory_buffer::from(ibo_data, sizeof(ibo_data));

    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(2);
    vbl.push<float>(2);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

gpu_mesh load_mesh(cpu_mesh mesh)
{
    uint32 vbo_id = 0;
    {
        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, mesh.vbo.size, mesh.vbo.data, GL_STATIC_DRAW);
    }

    uint32 ibo_id = 0;
    {
        glGenBuffers(1, &ibo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo.size, mesh.ibo.data, GL_STATIC_DRAW);
    }

    uint32 vao_id = 0;
    {
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        uint32 stride = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh.vbl.size; attrib_index++)
        {
            stride += (mesh.vbl.elements[attrib_index].count * sizeof(float32));
        }

        console::print("stride = %d\n", stride);

        usize offset = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh.vbl.size; attrib_index++)
        {
            uint32 count = mesh.vbl.elements[attrib_index].count;
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

    gpu_mesh result;
    result.vbo = vbo_id;
    result.ibo = ibo_id;
    result.vao = vao_id;
    result.count = (uint32) (mesh.ibo.size / sizeof(uint32));

    console::print("Mesh loaded: (vao = %d, vbo = %d, ibo = %d, count = %d)\n", vao_id, vbo_id, ibo_id, result.count);

    return result;
}

texture load_texture(bitmap tx)
{
    uint32 id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (tx.color_type == IMAGE_RGBA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tx.width, tx.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tx.pixels);
    else
        ASSERT_FAIL("Unsupported color type!");

    texture result;
    result.id = id;
    return result;
}

int compile_shader(char const *source_code, int shader_type)
{
    int id = glCreateShader(shader_type);
    glShaderSource(id, 1, (char const **) &source_code, (int const *) NULL);
    glCompileShader(id);

    int successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (successful == GL_FALSE)
    {
        isize length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, (GLint *) &length);

        auto message = mallocator().allocate_buffer(length + 1);

        glGetShaderInfoLog(id, length, (GLsizei *) &length, (char *) message.data);
        glDeleteShader(id);

        console::print("Could not compile shader: \"%s\"\n", (char *)message.data);
        mallocator().deallocate(message);

        return 0;
    }

    return id;
}

shader compile_shaders(char const *vs_code, char const *fs_code)
{
    int vs = compile_shader(vs_code, GL_VERTEX_SHADER);
    int fs = compile_shader(fs_code, GL_FRAGMENT_SHADER);

    int id = glCreateProgram();
    glUseProgram(0);
    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);
    glDetachShader(id, vs);
    glDetachShader(id, fs);

    shader result;
    result.id = id;
    result.vs_id = vs;
    result.fs_id = fs;

    console::print("Shaders compiled (id = %d, vs = %d, fs = %d)\n", id, vs, fs);

    return result;
}

framebuffer create_framebuffer(int width, int height)
{
    uint32 texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    uint32 render_buffer_id = 0;
    glGenRenderbuffers(1, &render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    uint32 id = 0;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto completenes_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (completenes_status)
    {
        case GL_FRAMEBUFFER_UNDEFINED: console::print("Specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: console::print("Any of the framebuffer attachment points are framebuffer incomplete.\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: console::print("The framebuffer does not have at least one image attached to it.\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: console::print("The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi.\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: console::print("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.\n"); break;
        case GL_FRAMEBUFFER_UNSUPPORTED: console::print("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: console::print("The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.\n");
            console::print("    OR\n");
            console::print("The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: console::print("Any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.\n"); break;
        case GL_FRAMEBUFFER_COMPLETE: console::print("Framebuffer created correctly\n"); break;
    }
    if (completenes_status != GL_FRAMEBUFFER_COMPLETE)
    {
        console::print("Error: Framebuffer is not complete!\n");
        return {};
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    framebuffer result;
    result.framebuffer_id = id;
    result.color_texture_id = texture_id;
    result.depth_stencil_id = render_buffer_id;

    return result;
}

#endif // COMMON_GRAPHICS_HPP

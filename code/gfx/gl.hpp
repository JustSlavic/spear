#ifndef GFX__GL_HPP
#define GFX__GL_HPP

#include <base.h>
#include <memory/buffer.hpp>
#include <memory/allocator.hpp>
#include "viewport.hpp"

#if OS_MAC
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_MULTISAMPLE                    0x809D
#define GL_MULTISAMPLE_ARB                0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLES_ARB                    0x80A9
#define GL_SAMPLES_EXT                    0x80A9
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46

/* ErrorCode */
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505

/* ClearBufferMask */
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define GL_FALSE                          0
#define GL_TRUE                           1

/* BeginMode */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006

/* DataType */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_FIXED                          0x140C

#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A

typedef uint8  GLboolean;
typedef uint32 GLenum;
typedef uint32 GLbitfield;
typedef float32 GLclampf;

#if DEBUG
#define GL_CHECK_ERRORS(...) { \
    auto err = glGetError();   \
    if (err) { ASSERT_FAIL(); } \
} void(0)
#else
#define GL_CHECK_ERRORS(...)
#endif


namespace gl
{

void clear_color(float32 r, float32 g, float32 b, float32 a)
{
    glClearColor(r, g, b, a);
    GL_CHECK_ERRORS();
}

void clear_color(vector4 color)
{
    clear_color(color.r, color.g, color.b, color.a);
}

void clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GL_CHECK_ERRORS();
}

void set_viewport(gfx::viewport vp)
{
    glViewport(vp.offset_x, vp.offset_y, vp.width, vp.height);
    GL_CHECK_ERRORS();
}

void depth_test(bool do_test)
{
    do_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void write_depth(bool do_write)
{
    glDepthMask(do_write ? GL_TRUE : GL_FALSE);
}

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
        GL_CHECK_ERRORS();
        glUniform1i(location, n);
        GL_CHECK_ERRORS();
    }

    void uniform(char const *name, float f)
    {
        auto location = glGetUniformLocation(id, name);
        GL_CHECK_ERRORS();
        glUniform1f(location, f);
        GL_CHECK_ERRORS();
    }

    void uniform(char const *name, vector2 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        GL_CHECK_ERRORS();
        glUniform2f(location, v.x, v.y);
        GL_CHECK_ERRORS();
    }

    void uniform(char const *name, vector3 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        GL_CHECK_ERRORS();
        glUniform3f(location, v.x, v.y, v.z);
        GL_CHECK_ERRORS();
    }

    void uniform(char const *name, vector4 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        GL_CHECK_ERRORS();
        glUniform4f(location, v.x, v.y, v.z, v.w);
        GL_CHECK_ERRORS();
    }

    void uniform(char const *name, matrix4 const& m)
    {
        auto location = glGetUniformLocation(id, name);
        GL_CHECK_ERRORS();
        glUniformMatrix4fv(location, 1, GL_TRUE, get_data(m));
        GL_CHECK_ERRORS();
    }
};

bool is_shader_program_valid(uint32 id)
{
    glValidateProgram(id);
    GL_CHECK_ERRORS();
    int32 is_program_valid;
    glGetProgramiv(id, GL_VALIDATE_STATUS, &is_program_valid);
    GL_CHECK_ERRORS();

    return is_program_valid;
}

uint32 compile_shader(memory_buffer source_code, shader::shader_type shader_type)
{
    uint32 id = glCreateShader(shader_type);
    GL_CHECK_ERRORS();
    glShaderSource(id, 1, (char const **) &source_code.data, (int const *) NULL);
    GL_CHECK_ERRORS();
    glCompileShader(id);
    GL_CHECK_ERRORS();

    int32 successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    GL_CHECK_ERRORS();
    if (successful == GL_FALSE)
    {
        int32 length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, (int32 *) &length);
        GL_CHECK_ERRORS();

        // @todo: use transient memory for that
        auto message = mallocator()->allocate_buffer(length + 1);
        memset(message.data, 0, length + 1);

        glGetShaderInfoLog(id, length, &length, (char *) message.data);
        GL_CHECK_ERRORS();

        glDeleteShader(id);
        GL_CHECK_ERRORS();

        mallocator()->deallocate(message);

        return 0;
    }

    return id;
}

uint32 link_shader(uint32 vs, uint32 fs)
{
    uint32 id = glCreateProgram();
    GL_CHECK_ERRORS();
    glUseProgram(0);
    GL_CHECK_ERRORS();
    glAttachShader(id, vs);
    GL_CHECK_ERRORS();
    glAttachShader(id, fs);
    GL_CHECK_ERRORS();
    glLinkProgram(id);
    GL_CHECK_ERRORS();
    glDetachShader(id, vs);
    GL_CHECK_ERRORS();
    glDetachShader(id, fs);
    GL_CHECK_ERRORS();

    if (is_shader_program_valid(id))
    {
        return id;
    }
    return 0;
}

void use_shader(shader s)
{
    glUseProgram(s.id);
    GL_CHECK_ERRORS();
}


uint32 load_texture(bitmap bitmap)
{
    uint32 id = 0;
    glGenTextures(1, &id);
    GL_CHECK_ERRORS();
    glBindTexture(GL_TEXTURE_2D, id);
    GL_CHECK_ERRORS();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GL_CHECK_ERRORS();

    if (bitmap.color_type == IMAGE_RGBA)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels);
    }
    else if (bitmap.color_type == IMAGE_RGB)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels);
    }
    else if (bitmap.color_type == IMAGE_GRAYSCALE)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels);
    }
    else if (bitmap.color_type == IMAGE_GRAYSCALE_ALPHA)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels);
    }
    // else if (bitmap.color_type == IMAGE_BGR)
    // {
    //     int32 swizzle_mask[4];
    //     swizzle_mask[0] = GL_BLUE;
    //     swizzle_mask[1] = GL_GREEN;
    //     swizzle_mask[2] = GL_RED;
    //     swizzle_mask[3] = GL_ONE;
    //     glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels);
    // }
    else
    {
        ASSERT_FAIL("Unsupported color type!");
    }
    GL_CHECK_ERRORS();

    return id;
}


} // namespace gl


#if OS_WINDOWS
#include "gl_win32.hpp"
#endif

#if OS_MAC
#include "gl_mac.hpp"
#endif

#endif // GFX__GL_HPP

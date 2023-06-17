#ifndef GFX_GL_HPP
#define GFX_GL_HPP

#include <base.hpp>
#include <GL/gl.h>

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

typedef void glGenFramebuffersType(isize n, uint32 *ids);
typedef void glBindFramebufferType(GLenum target, uint32 framebuffer);
typedef void glFramebufferTexture2DType(GLenum target, GLenum attachment, GLenum textarget, uint32 texture, int32 level);
typedef void glBlitFramebufferType(int32 srcX0, int32 srcY0, int32 srcX1, int32 srcY1, int32 dstX0, int32 dstY0, int32 dstX1, int32 dstY1, GLbitfield mask, GLenum filter);
typedef void glClearBufferivType(GLenum buffer, int32 drawbuffer, int32 const *value);
typedef void glGenBuffersType(isize n, uint32 *buffers);
typedef void glBindBufferType(GLenum target, uint32 buffer);
typedef void glBufferDataType(GLenum target, intptr size, const void *data, GLenum usage);
typedef void glGenVertexArraysType(isize n, uint32 *arrays);
typedef void glBindVertexArrayType(uint32 array);
typedef void glVertexAttribPointerType(uint32 index, int32 size, GLenum type, GLboolean normalized, isize stride, const void *pointer);
typedef void glEnableVertexAttribArrayType(uint32 index);
typedef uint32 glCreateShaderType(GLenum shaderType);
typedef void glShaderSourceType(uint32 shader, isize count, char const **string, int32 const *length);
typedef void glCompileShaderType(uint32 shader);
typedef uint32 glCreateProgramType(void);
typedef void glAttachShaderType(uint32 program, uint32 shader);
typedef void glDetachShaderType(uint32 program, uint32 shader);
typedef void glLinkProgramType(uint32 program);
typedef void glUseProgramType(uint32 program);
typedef void glGetShaderivType(uint32 shader, GLenum pname, int32 *params);
typedef void glGetShaderInfoLogType(uint32 shader, isize maxLength, isize *length, char *infoLog);
typedef void glDeleteShaderType(uint32 shader);
typedef void glValidateProgramType(uint32 program);
typedef void glGetProgramivType(uint32 program, GLenum pname, int32 *params);
typedef int32 glGetUniformLocationType(uint32 program, char const *uniform_name);
typedef void glUniform1iType(int32 location, int32 v0);
typedef void glUniform1fType(int32 location, float32 v0);
typedef void glUniform2fType(int32 location, float32 v0, float32 v1);
typedef void glUniform3fType(int32 location, float32 v0, float32 v1, float32 v2);
typedef void glUniform4fType(int32 location, float32 v0, float32 v1, float32 v2, float32 v3);
typedef void glUniformMatrix4fvType(int32 location, size_t count, bool transpose, float32 const *value);
typedef void glTexImage2DMultisampleType(GLenum target, isize samples, GLenum internalformat, isize width, isize height, GLboolean fixedsamplelocations);

GLOBAL glGenFramebuffersType *glGenFramebuffers;
GLOBAL glBindFramebufferType *glBindFramebuffer;
GLOBAL glFramebufferTexture2DType *glFramebufferTexture2D;
GLOBAL glBlitFramebufferType *glBlitFramebuffer;
GLOBAL glClearBufferivType *glClearBufferiv;
GLOBAL glGenBuffersType *glGenBuffers;
GLOBAL glBindBufferType *glBindBuffer;
GLOBAL glBufferDataType *glBufferData;
GLOBAL glGenVertexArraysType *glGenVertexArrays;
GLOBAL glBindVertexArrayType *glBindVertexArray;
GLOBAL glVertexAttribPointerType *glVertexAttribPointer;
GLOBAL glEnableVertexAttribArrayType *glEnableVertexAttribArray;
GLOBAL glCreateShaderType *glCreateShader;
GLOBAL glShaderSourceType *glShaderSource;
GLOBAL glCompileShaderType *glCompileShader;
GLOBAL glCreateProgramType *glCreateProgram;
GLOBAL glAttachShaderType *glAttachShader;
GLOBAL glDetachShaderType *glDetachShader;
GLOBAL glLinkProgramType *glLinkProgram;
GLOBAL glUseProgramType *glUseProgram;
GLOBAL glGetShaderivType *glGetShaderiv;
GLOBAL glGetShaderInfoLogType *glGetShaderInfoLog;
GLOBAL glDeleteShaderType *glDeleteShader;
GLOBAL glValidateProgramType *glValidateProgram;
GLOBAL glGetProgramivType *glGetProgramiv;
GLOBAL glGetUniformLocationType *glGetUniformLocation;
GLOBAL glUniform1iType *glUniform1i;
GLOBAL glUniform1fType *glUniform1f;
GLOBAL glUniform2fType *glUniform2f;
GLOBAL glUniform3fType *glUniform3f;
GLOBAL glUniform4fType *glUniform4f;
GLOBAL glUniformMatrix4fvType *glUniformMatrix4fv;
GLOBAL glTexImage2DMultisampleType *glTexImage2DMultisample;

#define GL_CHECK_ERRORS(...) { \
    auto err = glGetError();   \
    if (err) { ASSERT_FAIL(); } \
} void(0)

namespace gfx::gl {

char const *gl_error_string(GLenum ec)
{
    switch (ec) {
        case GL_INVALID_ENUM: return "Error: GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
        case GL_INVALID_VALUE: return "Error: GL_INVALID_VALUE: A numeric argument is out of range.";
        case GL_INVALID_OPERATION: return "Error: GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "Error: GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
        case GL_OUT_OF_MEMORY: return "Error: GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
        case GL_STACK_UNDERFLOW: return "Error: GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
        case GL_STACK_OVERFLOW: return "Error: GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
        case GL_NO_ERROR: return NULL;  // No error has been recorded. The value of this symbolic constant is guaranteed to be 0.
    }

    return NULL;
}

void set_clear_color(float32 r, float32 g, float32 b, float32 a)
{
    glClearColor(r, g, b, a);
    GL_CHECK_ERRORS();
}

void set_clear_color(math::vector4 color)
{
    set_clear_color(color.r, color.g, color.b, color.a);
}

void clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GL_CHECK_ERRORS();
}


struct shader
{
    uint32 id;
    uint32 vertex_shader;
    uint32 fragment_shader;

    enum shader_type
    {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
    };
};


bool32 is_shader_program_valid(uint32 program)
{
    glValidateProgram(program);
    GL_CHECK_ERRORS();
    bool32 program_valid;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &program_valid);
    GL_CHECK_ERRORS();

    return program_valid;
}

uint32 compile_shader(char const *source_code, shader::shader_type shader_type)
{
    uint32 id = glCreateShader(shader_type);
    GL_CHECK_ERRORS();
    glShaderSource(id, 1, &source_code, NULL);
    GL_CHECK_ERRORS();
    glCompileShader(id);
    GL_CHECK_ERRORS();

    int32 successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    GL_CHECK_ERRORS();
    if (successful == GL_FALSE)
    {
        int64 length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, (int32 *) &length);
        GL_CHECK_ERRORS();

        // @todo: use transient memory for that
        char* message = new char[length + 1];
        memory::set(message, 0, length + 1);

        glGetShaderInfoLog(id, length, &length, message);
        GL_CHECK_ERRORS();

        // osOutputDebugString("%s", message);

        glDeleteShader(id);
        GL_CHECK_ERRORS();
        delete[] message;

        return 0;
    }

    return id;
}

shader link_shader(uint32 vs, uint32 fs)
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

    shader result = {};
    if (is_shader_program_valid(id))
    {
        result.id = id;
        result.vertex_shader = vs;
        result.fragment_shader = fs;
    }
    else
    {
        // @todo: process error
    }

    return result;
}

void use_shader(shader s)
{
    glUseProgram(s.id);
    GL_CHECK_ERRORS();
}

void uniform(shader s, char const *name, int32 n)
{
    auto location = glGetUniformLocation(s.id, name);
    GL_CHECK_ERRORS();
    glUniform1i(location, n);
    GL_CHECK_ERRORS();
}

void uniform(shader s, char const *name, float f)
{
    auto location = glGetUniformLocation(s.id, name);
    GL_CHECK_ERRORS();
    glUniform1f(location, f);
    GL_CHECK_ERRORS();
}

void uniform(shader s, char const *name, math::vector2 const& v)
{
    auto location = glGetUniformLocation(s.id, name);
    GL_CHECK_ERRORS();
    glUniform2f(location, v.x, v.y);
    GL_CHECK_ERRORS();
}

void uniform(shader s, char const *name, math::vector3 const& v)
{
    auto location = glGetUniformLocation(s.id, name);
    GL_CHECK_ERRORS();
    glUniform3f(location, v.x, v.y, v.z);
    GL_CHECK_ERRORS();
}

void uniform(shader s, char const *name, math::vector4 const& v)
{
    auto location = glGetUniformLocation(s.id, name);
    GL_CHECK_ERRORS();
    glUniform4f(location, v.x, v.y, v.z, v.w);
    GL_CHECK_ERRORS();
}

void uniform(shader s, char const *name, math::matrix4 const& m)
{
    auto location = glGetUniformLocation(s.id, name);
    GL_CHECK_ERRORS();
    glUniformMatrix4fv(location, 1, GL_FALSE, m.data());
    GL_CHECK_ERRORS();
}

void set_viewport(viewport vp)
{
    glViewport(vp.offset_x, vp.offset_y, vp.width, vp.height);
    GL_CHECK_ERRORS();
}


} // namespace gfx::gl

#if OS_WINDOWS
#include "gl_win32.hpp"
#endif

#if OS_LINUX
#include "gl_x11.hpp"
#endif

#endif // GFX_GL_HPP

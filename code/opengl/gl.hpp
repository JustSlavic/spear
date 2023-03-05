#ifndef OPENGL_GL_HPP
#define OPENGL_GL_HPP

#include <base.hpp>
#include <math/vector3.hpp>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>

#include <gl/gl.h>


#if OS_WINDOWS

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef const char *WINAPI wglGetExtensionsStringARBType(HDC hdc);
typedef BOOL WINAPI wglChoosePixelFormatARBType(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC WINAPI wglCreateContextAttribsARBType(HDC hdc, HGLRC hShareContext, int const *attribList);
typedef BOOL wglSwapIntervalEXTType(int interval);
typedef int wglGetSwapIntervalEXTType(void);

GLOBAL wglGetExtensionsStringARBType *wglGetExtensionsStringARB;
GLOBAL wglChoosePixelFormatARBType *wglChoosePixelFormatARB;
GLOBAL wglCreateContextAttribsARBType *wglCreateContextAttribsARB;
GLOBAL wglSwapIntervalEXTType *wglSwapIntervalEXT;
GLOBAL wglGetSwapIntervalEXTType *wglGetSwapIntervalEXT;

#endif // OS_WINDOWS


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


typedef void glGenFramebuffersType(GLsizei n, GLuint *ids);
typedef void glBindFramebufferType(GLenum target, GLuint framebuffer);
typedef void glFramebufferTexture2DType(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void glBlitFramebufferType(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void glClearBufferivType(GLenum buffer, GLint drawbuffer, GLint const *value);
typedef void glActiveTextureType(GLenum texture);
typedef void glGenBuffersType(GLsizei n, GLuint *buffers);
typedef void glBindBufferType(GLenum target, GLuint buffer);
typedef void glBufferDataType(GLenum target, intptr_t size, const void *data, GLenum usage);
typedef void glGenVertexArraysType(GLsizei n, GLuint *arrays);
typedef void glBindVertexArrayType(GLuint array);
typedef void glVertexAttribPointerType(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void glEnableVertexAttribArrayType(GLuint index);
typedef GLuint glCreateShaderType(GLenum shaderType);
typedef void glShaderSourceType(GLuint shader, GLsizei count, char const **string, GLint const *length);
typedef void glCompileShaderType(GLuint shader);
typedef GLuint glCreateProgramType(void);
typedef void glAttachShaderType(GLuint program, GLuint shader);
typedef void glDetachShaderType(GLuint program, GLuint shader);
typedef void glLinkProgramType(GLuint program);
typedef void glUseProgramType(GLuint program);
typedef void glGetShaderivType(GLuint shader, GLenum pname, GLint *params);
typedef void glGetShaderInfoLogType(GLuint shader, GLsizei maxLength, GLsizei *length, char *infoLog);
typedef void glDeleteShaderType(GLuint shader);
typedef void glValidateProgramType(GLuint program);
typedef void glGetProgramivType(GLuint program, GLenum pname, GLint *params);
typedef GLint glGetUniformLocationType(GLuint program, char const *uniform_name);
typedef void glUniform1iType(GLint location, GLint v0);
typedef void glUniform1fType(GLint location, GLfloat v0);
typedef void glUniform2fType(GLint location, GLfloat v0, GLfloat v1);
typedef void glUniform3fType(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void glUniform4fType(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void glUniformMatrix4fvType(int32_t location, size_t count, bool transpose, float32 const *value);
typedef void glTexImage2DMultisampleType(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);


GLOBAL glGenFramebuffersType *glGenFramebuffers;
GLOBAL glBindFramebufferType *glBindFramebuffer;
GLOBAL glFramebufferTexture2DType *glFramebufferTexture2D;
GLOBAL glBlitFramebufferType *glBlitFramebuffer;
GLOBAL glClearBufferivType *glClearBufferiv;
GLOBAL glActiveTextureType *glActiveTexture;
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


namespace gl
{

#if OS_WINDOWS

void initialize()
{
    glGenFramebuffers = (glGenFramebuffersType *) wglGetProcAddress("glGenFramebuffers");
    glBindFramebuffer = (glBindFramebufferType *) wglGetProcAddress("glBindFramebuffer");
    glFramebufferTexture2D = (glFramebufferTexture2DType *) wglGetProcAddress("glFramebufferTexture2D");
    glBlitFramebuffer = (glBlitFramebufferType *) wglGetProcAddress("glBlitFramebuffer");
    glClearBufferiv = (glClearBufferivType *) wglGetProcAddress("glClearBufferiv");
    glActiveTexture = (glActiveTextureType *) wglGetProcAddress("glActiveTexture");
    glGenBuffers = (glGenBuffersType *) wglGetProcAddress("glGenBuffers");
    glBindBuffer = (glBindBufferType *) wglGetProcAddress("glBindBuffer");
    glBufferData = (glBufferDataType *) wglGetProcAddress("glBufferData");
    glGenVertexArrays = (glGenVertexArraysType *) wglGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (glBindVertexArrayType *) wglGetProcAddress("glBindVertexArray");
    glVertexAttribPointer = (glVertexAttribPointerType *) wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (glEnableVertexAttribArrayType *) wglGetProcAddress("glEnableVertexAttribArray");
    glCreateShader = (glCreateShaderType *) wglGetProcAddress("glCreateShader");
    glShaderSource = (glShaderSourceType *) wglGetProcAddress("glShaderSource");
    glCompileShader = (glCompileShaderType *) wglGetProcAddress("glCompileShader");
    glCreateProgram = (glCreateProgramType *) wglGetProcAddress("glCreateProgram");
    glAttachShader = (glAttachShaderType *) wglGetProcAddress("glAttachShader");
    glDetachShader = (glDetachShaderType *) wglGetProcAddress("glDetachShader");
    glLinkProgram = (glLinkProgramType *) wglGetProcAddress("glLinkProgram");
    glUseProgram = (glUseProgramType *) wglGetProcAddress("glUseProgram");
    glGetShaderiv = (glGetShaderivType *) wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (glGetShaderInfoLogType *) wglGetProcAddress("glGetShaderInfoLog");
    glDeleteShader = (glDeleteShaderType *) wglGetProcAddress("glDeleteShader");
    glValidateProgram = (glValidateProgramType *) wglGetProcAddress("glValidateProgram");
    glGetProgramiv = (glGetProgramivType *) wglGetProcAddress("glGetProgramiv");
    glGetUniformLocation = (glGetUniformLocationType *) wglGetProcAddress("glGetUniformLocation");
    glUniform1i = (glUniform1iType *) wglGetProcAddress("glUniform1i");
    glUniform1f = (glUniform1fType *) wglGetProcAddress("glUniform1f");
    glUniform2f = (glUniform2fType *) wglGetProcAddress("glUniform2f");
    glUniform3f = (glUniform3fType *) wglGetProcAddress("glUniform3f");
    glUniform4f = (glUniform4fType *) wglGetProcAddress("glUniform4f");
    glUniformMatrix4fv = (glUniformMatrix4fvType *) wglGetProcAddress("glUniformMatrix4fv");
    glTexImage2DMultisample = (glTexImage2DMultisampleType *) wglGetProcAddress("glTexImage2DMultisample");
}


void set_clear_color(float32 r, float32 g, float32 b, float32 a)
{
    glClearColor(r, g, b, a);
}


void set_clear_color(math::vector4 color)
{
    set_clear_color(color.r, color.g, color.b, color.a);
}


void clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void vsync(bool turn_on)
{
    wglSwapIntervalEXT(turn_on ? 1 : 0);
}


#endif // OS_WINDOWS


#define GL_CHECK_ERRORS(...) { \
    auto err = glGetError();   \
    if (err) { ASSERT_FAIL(); } \
} void(0)


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


struct shader
{
    uint32_t id;
    uint32_t vertex_shader;
    uint32_t fragment_shader;

    enum shader_type
    {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
    };
};


bool32_t is_shader_program_valid(uint32_t program)
{
    glValidateProgram(program);
    bool32_t program_valid;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &program_valid);

    return program_valid;
}


uint32_t compile_shader(char const *source_code, shader::shader_type shader_type)
{
    uint32_t id = glCreateShader(shader_type);
    glShaderSource(id, 1, &source_code, NULL);
    glCompileShader(id);

    int32_t successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (successful == GL_FALSE)
    {
        int32_t length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // @todo: use transient memory for that
        char* message = new char[length + 1];
        memset(message, 0, length + 1);

        glGetShaderInfoLog(id, length, &length, message);

        // osOutputDebugString("%s", message);

        glDeleteShader(id);
        delete[] message;

        return 0;
    }

    return id;
}


shader link_shader(uint32_t vs, uint32_t fs)
{
    uint32_t id = glCreateProgram();
    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);
    glDetachShader(id, vs);
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
}


void uniform(shader s, char const *name, float f)
{
    auto location = glGetUniformLocation(s.id, name);
    glUniform1f(location, f);
}


void uniform(shader s, char const *name, math::vector2 const& v)
{
    auto location = glGetUniformLocation(s.id, name);
    glUniform2f(location, v.x, v.y);
}


void uniform(shader s, char const *name, math::vector3 const& v)
{
    auto location = glGetUniformLocation(s.id, name);
    glUniform3f(location, v.x, v.y, v.z);
}


void uniform(shader s, char const *name, math::vector4 const& v)
{
    auto location = glGetUniformLocation(s.id, name);
    glUniform4f(location, v.x, v.y, v.z, v.w);
}


void uniform(shader s, char const *name, math::matrix4 const& m)
{
    auto location = glGetUniformLocation(s.id, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, m.data());
}


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

    float32 tf2 = (1.0f / math::tangent(0.5f * fov));

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


math::matrix4 make_look_at_matrix(math::vector3 eye, math::vector3 at, math::vector3 up)
{
    using namespace math;

    vector3 f = normalized(at - eye);
    vector3 s = normalized(cross(f, up));
    vector3 u = cross(s, f);

    matrix4 result =
    {
         s.x,  u.x,  -f.x, 0,
         s.y,  u.y,  -f.y, 0,
         s.z,  u.z,  -f.z, 0,
        -dot(s, eye), -dot(u, eye), dot(f, eye), 1,
    };

    return result;
}


struct viewport
{
    uint32_t offset_x;
    uint32_t offset_y;
    uint32_t width;
    uint32_t height;
};


viewport make_viewport(uint32_t width, uint32_t height, float32 desired_aspect_ratio)
{
    viewport result;

    float32 aspect_ratio = (float32) width / (float32) height;
    if (aspect_ratio < desired_aspect_ratio)
    {
        // Black strips on top and bottom of the screen
        result.width    = width;
        result.height   = (uint32_t) (result.width / desired_aspect_ratio);
        result.offset_x = 0;
        result.offset_y = (height - result.height) / 2;
    }
    else if (aspect_ratio > desired_aspect_ratio)
    {
        // Black strips on left and right of the screen
        result.height   = height;
        result.width    = (uint32_t) (result.height * desired_aspect_ratio);
        result.offset_x = (width - result.width) / 2;
        result.offset_y = 0;
    }
    else
    {
        // No black strips
        result.width    = width;
        result.height   = height;
        result.offset_x = 0;
        result.offset_y = 0;
    }

    return result;
}


void set_viewport(viewport vp)
{
    glViewport(vp.offset_x, vp.offset_y, vp.width, vp.height);
}


} // namespace gl


#endif // OPENGL_GL_HPP

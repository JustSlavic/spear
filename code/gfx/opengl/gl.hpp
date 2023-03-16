#ifndef OPENGL_GL_HPP
#define OPENGL_GL_HPP

#include <base.hpp>
#include <memory/memory.hpp>
#include <math/vector3.hpp>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>
#include <gfx/viewport.hpp>

#if OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#endif // OS_WiNDOWS
#if OS_LINUX
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#endif // OS_LINUX

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

#if OS_LINUX

typedef struct __GLXFBConfigRec *GLXFBConfig;
// typedef struct __GFXcontextRec  *GLXContext;

// typedef XVisualInfo *( *PFNGLXGETVISUALFROMFBCONFIGPROC) (Display *dpy, GLXFBConfig config);

#define GLX_WINDOW_BIT                    0x00000001
#define GLX_RGBA_BIT                      0x00000001

#define GLX_X_VISUAL_TYPE                 0x22

#define GLX_TRUE_COLOR                    0x8002
#define GLX_DRAWABLE_TYPE                 0x8010
#define GLX_RENDER_TYPE                   0x8011
#define GLX_X_RENDERABLE                  0x8012

#define GLX_RGBA                          4
#define GLX_DOUBLEBUFFER                  5

#define GLX_RED_SIZE                      8
#define GLX_GREEN_SIZE                    9
#define GLX_BLUE_SIZE                     10
#define GLX_ALPHA_SIZE                    11
#define GLX_DEPTH_SIZE                    12
#define GLX_STENCIL_SIZE                  13
// #define GLX_SAMPLE_BUFFERS                100000
// #define GLX_SAMPLES                       100001

extern "C"
{
// bool glXQueryVersion(Display *dpy, int *major, int *minor);
GLXFBConfig *glXChooseFBConfig (Display *dpy, int screen, const int *attrib_list, int *nelements);
XVisualInfo *glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config);
int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
const char *glXQueryExtensionsString( Display *dpy, int screen );
}

typedef void glXSwapIntervalEXTType(Display *display, GLXDrawable drawable, int interval);

GLOBAL glXSwapIntervalEXTType *glXSwapIntervalEXT;

#endif // OS_LINUX


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
typedef void glUniformMatrix4fvType(int32 location, size_t count, bool transpose, float32 const *value);
typedef void glTexImage2DMultisampleType(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);


GLOBAL glGenFramebuffersType *glGenFramebuffers;
GLOBAL glBindFramebufferType *glBindFramebuffer;
GLOBAL glFramebufferTexture2DType *glFramebufferTexture2D;
GLOBAL glBlitFramebufferType *glBlitFramebuffer;
GLOBAL glClearBufferivType *glClearBufferiv;
// GLOBAL glActiveTextureType *glActiveTexture;
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

namespace gfx {
namespace gl {

#if OS_WINDOWS

bool32 initialize()
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

    return true;
}

void vsync(bool turn_on)
{
    wglSwapIntervalEXT(turn_on ? 1 : 0);
}

#endif // OS_WINDOWS

#if OS_LINUX

bool32 initialize()
{
    glGenFramebuffers = (glGenFramebuffersType *) glXGetProcAddress((GLubyte const *) "glGenFramebuffers");
    glBindFramebuffer = (glBindFramebufferType *) glXGetProcAddress((GLubyte const *) "glBindFramebuffer");
    glFramebufferTexture2D = (glFramebufferTexture2DType *) glXGetProcAddress((GLubyte const *) "glFramebufferTexture2D");
    glBlitFramebuffer = (glBlitFramebufferType *) glXGetProcAddress((GLubyte const *) "glBlitFramebuffer");
    glClearBufferiv = (glClearBufferivType *) glXGetProcAddress((GLubyte const *) "glClearBufferiv");
    // glActiveTexture = (glActiveTextureType *) glXGetProcAddress((GLubyte const *) "glActiveTexture");
    glGenBuffers = (glGenBuffersType *) glXGetProcAddress((GLubyte const *) "glGenBuffers");
    glBindBuffer = (glBindBufferType *) glXGetProcAddress((GLubyte const *) "glBindBuffer");
    glBufferData = (glBufferDataType *) glXGetProcAddress((GLubyte const *) "glBufferData");
    glGenVertexArrays = (glGenVertexArraysType *) glXGetProcAddress((GLubyte const *) "glGenVertexArrays");
    glBindVertexArray = (glBindVertexArrayType *) glXGetProcAddress((GLubyte const *) "glBindVertexArray");
    glVertexAttribPointer = (glVertexAttribPointerType *) glXGetProcAddress((GLubyte const *) "glVertexAttribPointer");
    glEnableVertexAttribArray = (glEnableVertexAttribArrayType *) glXGetProcAddress((GLubyte const *) "glEnableVertexAttribArray");
    glCreateShader = (glCreateShaderType *) glXGetProcAddress((GLubyte const *) "glCreateShader");
    glShaderSource = (glShaderSourceType *) glXGetProcAddress((GLubyte const *) "glShaderSource");
    glCompileShader = (glCompileShaderType *) glXGetProcAddress((GLubyte const *) "glCompileShader");
    glCreateProgram = (glCreateProgramType *) glXGetProcAddress((GLubyte const *) "glCreateProgram");
    glAttachShader = (glAttachShaderType *) glXGetProcAddress((GLubyte const *) "glAttachShader");
    glDetachShader = (glDetachShaderType *) glXGetProcAddress((GLubyte const *) "glDetachShader");
    glLinkProgram = (glLinkProgramType *) glXGetProcAddress((GLubyte const *) "glLinkProgram");
    glUseProgram = (glUseProgramType *) glXGetProcAddress((GLubyte const *) "glUseProgram");
    glGetShaderiv = (glGetShaderivType *) glXGetProcAddress((GLubyte const *) "glGetShaderiv");
    glGetShaderInfoLog = (glGetShaderInfoLogType *) glXGetProcAddress((GLubyte const *) "glGetShaderInfoLog");
    glDeleteShader = (glDeleteShaderType *) glXGetProcAddress((GLubyte const *) "glDeleteShader");
    glValidateProgram = (glValidateProgramType *) glXGetProcAddress((GLubyte const *) "glValidateProgram");
    glGetProgramiv = (glGetProgramivType *) glXGetProcAddress((GLubyte const *) "glGetProgramiv");
    glGetUniformLocation = (glGetUniformLocationType *) glXGetProcAddress((GLubyte const *) "glGetUniformLocation");
    glUniform1i = (glUniform1iType *) glXGetProcAddress((GLubyte const *) "glUniform1i");
    glUniform1f = (glUniform1fType *) glXGetProcAddress((GLubyte const *) "glUniform1f");
    glUniform2f = (glUniform2fType *) glXGetProcAddress((GLubyte const *) "glUniform2f");
    glUniform3f = (glUniform3fType *) glXGetProcAddress((GLubyte const *) "glUniform3f");
    glUniform4f = (glUniform4fType *) glXGetProcAddress((GLubyte const *) "glUniform4f");
    glUniformMatrix4fv = (glUniformMatrix4fvType *) glXGetProcAddress((GLubyte const *) "glUniformMatrix4fv");
    glTexImage2DMultisample = (glTexImage2DMultisampleType *) glXGetProcAddress((GLubyte const *) "glTexImage2DMultisample");

    return true;
    UNUSED(glTexImage2DMultisample);
    UNUSED(glUniform1i);
    UNUSED(glEnableVertexAttribArray);
    UNUSED(glBindFramebuffer);
    UNUSED(glGenFramebuffers);
    UNUSED(glBlitFramebuffer);
    UNUSED(glClearBufferiv);
    UNUSED(glGenBuffers);
    UNUSED(glFramebufferTexture2D);
    UNUSED(glBufferData);
    UNUSED(glBindBuffer);
    UNUSED(glGenVertexArrays);
    UNUSED(glVertexAttribPointer);
    UNUSED(glBindVertexArray);
    return false;
}

void vsync(bool turn_on)
{

}

#endif // OS_LINUX


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
    bool32 program_valid;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &program_valid);

    return program_valid;
}


uint32 compile_shader(char const *source_code, shader::shader_type shader_type)
{
    uint32 id = glCreateShader(shader_type);
    glShaderSource(id, 1, &source_code, NULL);
    glCompileShader(id);

    int32 successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (successful == GL_FALSE)
    {
        int32 length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // @todo: use transient memory for that
        char* message = new char[length + 1];
        memory::set(message, 0, length + 1);

        glGetShaderInfoLog(id, length, &length, message);

        // osOutputDebugString("%s", message);

        glDeleteShader(id);
        delete[] message;

        return 0;
    }

    return id;
}


shader link_shader(uint32 vs, uint32 fs)
{
    uint32 id = glCreateProgram();
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

void set_viewport(viewport vp)
{
    glViewport(vp.offset_x, vp.offset_y, vp.width, vp.height);
}


} // namespace gl
} // namespace gfx


#endif // OPENGL_GL_HPP

#include <base.h>
#include <string_view.hpp>
#include <platform.hpp>
#include <memory/allocator.hpp>
#include <math/matrix4.hpp>
#include <console.hpp>
#include <image/png.hpp>
#include <memory/serializer.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>

#define MAIN_WINDOW_CALLBACK(NAME) LRESULT CALLBACK NAME(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
typedef MAIN_WINDOW_CALLBACK(MainWindowCallbackType);

#include <input.hpp>
#include <gen/font_14x26.hpp>
#include <game_interface.hpp>
#include <gfx/static_shaders.cpp>


GLOBAL uint32 current_client_width;
GLOBAL uint32 current_client_height;


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
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
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
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100

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

typedef void glActiveTextureType(GLenum texture);
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
typedef void glGetShaderInfoLogType(uint32 shader, GLsizei maxLength, GLsizei *length, char *infoLog); // GLsizei is int on windows and int64 on Mac?
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
typedef void glGenRenderbuffersType(isize n, uint32 *renderbuffers);
typedef void glBindRenderbufferType(GLenum target, uint32 renderbuffer);
typedef void glRenderbufferStorageType(GLenum target, GLenum internalformat, isize width, isize height);
typedef void glFramebufferRenderbufferType(GLenum target, GLenum attachment, GLenum renderbuffertarget, uint32 renderbuffer);
typedef GLenum glCheckFramebufferStatusType(GLenum target);

GLOBAL glActiveTextureType *glActiveTexture;
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
GLOBAL glGenRenderbuffersType *glGenRenderbuffers;
GLOBAL glBindRenderbufferType *glBindRenderbuffer;
GLOBAL glRenderbufferStorageType *glRenderbufferStorage;
GLOBAL glFramebufferRenderbufferType *glFramebufferRenderbuffer;
GLOBAL glCheckFramebufferStatusType *glCheckFramebufferStatus;

bool32 initialize_opengl()
{
#define WGL_GET_PROC_ADDRESS(NAME) NAME = (NAME##Type *) wglGetProcAddress(STRINGIFY(NAME)); \
    if (NAME == NULL) return false;

    WGL_GET_PROC_ADDRESS(glGenFramebuffers);
    WGL_GET_PROC_ADDRESS(glBindFramebuffer);
    WGL_GET_PROC_ADDRESS(glFramebufferTexture2D);
    WGL_GET_PROC_ADDRESS(glBlitFramebuffer);
    WGL_GET_PROC_ADDRESS(glClearBufferiv);
    WGL_GET_PROC_ADDRESS(glActiveTexture);
    WGL_GET_PROC_ADDRESS(glGenBuffers);
    WGL_GET_PROC_ADDRESS(glBindBuffer);
    WGL_GET_PROC_ADDRESS(glBufferData);
    WGL_GET_PROC_ADDRESS(glGenVertexArrays);
    WGL_GET_PROC_ADDRESS(glBindVertexArray);
    WGL_GET_PROC_ADDRESS(glVertexAttribPointer);
    WGL_GET_PROC_ADDRESS(glEnableVertexAttribArray);
    WGL_GET_PROC_ADDRESS(glCreateShader);
    WGL_GET_PROC_ADDRESS(glShaderSource);
    WGL_GET_PROC_ADDRESS(glCompileShader);
    WGL_GET_PROC_ADDRESS(glCreateProgram);
    WGL_GET_PROC_ADDRESS(glAttachShader);
    WGL_GET_PROC_ADDRESS(glDetachShader);
    WGL_GET_PROC_ADDRESS(glLinkProgram);
    WGL_GET_PROC_ADDRESS(glUseProgram);
    WGL_GET_PROC_ADDRESS(glGetShaderiv);
    WGL_GET_PROC_ADDRESS(glGetShaderInfoLog);
    WGL_GET_PROC_ADDRESS(glDeleteShader);
    WGL_GET_PROC_ADDRESS(glValidateProgram);
    WGL_GET_PROC_ADDRESS(glGetProgramiv);
    WGL_GET_PROC_ADDRESS(glGetUniformLocation);
    WGL_GET_PROC_ADDRESS(glUniform1i);
    WGL_GET_PROC_ADDRESS(glUniform1f);
    WGL_GET_PROC_ADDRESS(glUniform2f);
    WGL_GET_PROC_ADDRESS(glUniform3f);
    WGL_GET_PROC_ADDRESS(glUniform4f);
    WGL_GET_PROC_ADDRESS(glUniformMatrix4fv);
    WGL_GET_PROC_ADDRESS(glTexImage2DMultisample);
    WGL_GET_PROC_ADDRESS(glGenRenderbuffers);
    WGL_GET_PROC_ADDRESS(glBindRenderbuffer);
    WGL_GET_PROC_ADDRESS(glRenderbufferStorage);
    WGL_GET_PROC_ADDRESS(glFramebufferRenderbuffer);
    WGL_GET_PROC_ADDRESS(glCheckFramebufferStatus);

#undef WGL_GET_PROC_ADDRESS

    return true;
}

#if DEBUG
const char* gl_get_error_string(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:          return "GL_NO_ERROR";
        case GL_INVALID_ENUM:      return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:     return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW:    return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:   return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:     return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:                   return "UNKNOWN_ERROR";
    }
}

#define GL_CHECK_ERRORS(...) { \
    auto err = glGetError();   \
    if (err) { console::print("%s\n", gl_get_error_string(err)); ASSERT_FAIL(); } \
} void(0)
#else
#define GL_CHECK_ERRORS(...)
#endif

#include <common_graphics.hpp>
#include <platonic_solids.hpp>

#include "game_loop.hpp"
#include "debug_graph.hpp"


GLOBAL game_loop_data game_loop;


void draw_platonic_solid(gpu_mesh mesh, shader s, vector4 c, matrix4 m, matrix4 v, matrix4 p)
{
    glUseProgram(s.id);

    s.uniform("u_model", m);
    s.uniform("u_view", v);
    s.uniform("u_projection", p);
    s.uniform("u_color", c);

    glBindVertexArray(mesh.vao);
    if (mesh.ibo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
        glDrawElements(GL_TRIANGLES, mesh.count, GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh.count);
    }
}

namespace win32 {


int32 get_width(RECT rectangle)
        {
    int32 result = rectangle.right - rectangle.left;
    return result;
}

int32 get_height(RECT rectangle)
{
    int32 result = rectangle.bottom - rectangle.top;
    return result;
}

struct window
{
    HWND Handle;
    HDC DeviceContext;
};


uint32 get_program_path(HINSTANCE instance, char *buffer, uint32 buffer_size)
{
    uint32 result_size = GetModuleFileNameA(instance, buffer, buffer_size);
    return result_size;
}

uint64 get_file_time(char const *file_path)
{
    WIN32_FILE_ATTRIBUTE_DATA file_data;
    GetFileAttributesExA(file_path, GetFileExInfoStandard, &file_data);
    FILETIME file_time = file_data.ftLastWriteTime;
    uint64 result = ((uint64) file_time.dwLowDateTime) | (((uint64) file_time.dwHighDateTime) << 32);
    return result;
}

struct dll
{
    HMODULE handle;
    uint64 timestamp;

    static dll load(char const *file_path)
    {
        dll result;
        result.handle = LoadLibraryA(file_path);
        if (result.handle)
        {
            result.timestamp = get_file_time(file_path);
        }
        return result;
    }

    void unload()
    {
        if (handle)
        {
            FreeLibrary(handle);
            handle = NULL;
        }
    }

    [[nodiscard]]
    void *get_function(char const *name)
    {
        void *address = GetProcAddress(handle, name);
        return address;
    }
};

bool32 create_window(HINSTANCE Instance, int32 ClientWidth, int32 ClientHeight, MainWindowCallbackType *WindowCallback, window *result)
{
    int32 PrimaryMonitorWidth  = GetSystemMetrics(SM_CXSCREEN);
    int32 PrimaryMonitorHeight = GetSystemMetrics(SM_CYSCREEN);

    HBRUSH BlackBrush = CreateSolidBrush(RGB(0, 0, 0));

    WNDCLASSA WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "WindowClass";
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = BlackBrush;

    ATOM WindowClassAtom = RegisterClassA(&WindowClass);
    if (!WindowClassAtom)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not register window class.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    RECT WindowRectangle = { 0, 0, (int) ClientWidth, (int) ClientHeight };
    if (!AdjustWindowRect(&WindowRectangle, WS_OVERLAPPEDWINDOW, false))
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! AdjustWindowRect failed.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    HWND window = CreateWindowExA(0, WindowClass.lpszClassName, "OpenGL temp window", WS_OVERLAPPEDWINDOW, 0, 0, 50, 50, 0, 0, Instance, NULL);
    if (!window)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not create a window.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    HDC device_context = GetDC(window);
    {
        HGLRC temp_render_context = {};
        HGLRC render_context = {};

        PIXELFORMATDESCRIPTOR desired_pixel_format = {};
        desired_pixel_format.nSize = sizeof(desired_pixel_format);
        desired_pixel_format.nVersion = 1;
        desired_pixel_format.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
        desired_pixel_format.iPixelType = PFD_TYPE_RGBA;
        desired_pixel_format.cColorBits = 24;
        desired_pixel_format.cAlphaBits = 8;
        desired_pixel_format.cDepthBits = 24;
        desired_pixel_format.cStencilBits = 8;
        desired_pixel_format.iLayerType = PFD_MAIN_PLANE;

        int suggested_pixel_format_index = ChoosePixelFormat(device_context, &desired_pixel_format);
        PIXELFORMATDESCRIPTOR suggested_pixel_format;
        DescribePixelFormat(device_context, suggested_pixel_format_index, sizeof(suggested_pixel_format), &suggested_pixel_format);

        if (SetPixelFormat(device_context, suggested_pixel_format_index, &suggested_pixel_format))
        {
            temp_render_context = wglCreateContext(device_context);
            if (wglMakeCurrent(device_context, temp_render_context))
            {
                wglGetExtensionsStringARB = (wglGetExtensionsStringARBType *) wglGetProcAddress("wglGetExtensionsStringARB");
                wglChoosePixelFormatARB = (wglChoosePixelFormatARBType *) wglGetProcAddress("wglChoosePixelFormatARB");
                wglCreateContextAttribsARB = (wglCreateContextAttribsARBType *) wglGetProcAddress("wglCreateContextAttribsARB");
                // @todo Check if 'WGL_EXT_swap_control' extension is available
                wglSwapIntervalEXT = (wglSwapIntervalEXTType *) wglGetProcAddress("wglSwapIntervalEXT");
                wglGetSwapIntervalEXT = (wglGetSwapIntervalEXTType *) wglGetProcAddress("wglGetSwapIntervalEXT");

                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(temp_render_context);
                ReleaseDC(window, device_context);
                DestroyWindow(window);

                window = CreateWindowExA(
                    0, WindowClass.lpszClassName,
                    "window",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    (PrimaryMonitorWidth - win32::get_width(WindowRectangle)) / 2,
                    (PrimaryMonitorHeight - win32::get_height(WindowRectangle)) / 2,
                    win32::get_width(WindowRectangle),
                    win32::get_height(WindowRectangle),
                    0, 0, Instance, NULL);

                device_context = GetDC(window);

                // char const *WglExtensionString = wglGetExtensionsStringARB(DeviceContext);
                // Check if extension is available in the string
                // OutputDebugStringA(WglExtensionString);

                int wgl_attribute_list[] =
                {
                    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                    WGL_COLOR_BITS_ARB, 32,
                    WGL_DEPTH_BITS_ARB, 24,
                    WGL_STENCIL_BITS_ARB, 8,
                    WGL_SAMPLE_BUFFERS_ARB, 1,
                    WGL_SAMPLES_ARB, 4,
                    0, // End
                };

                int32 pixel_format;
                uint32 number_formats;
                wglChoosePixelFormatARB(device_context, wgl_attribute_list, NULL, 1, &pixel_format, &number_formats);
                SetPixelFormat(device_context, pixel_format, &desired_pixel_format);

                int32 wgl_context_attrib_list[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                    WGL_CONTEXT_MINOR_VERSION_ARB, 0,
                    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                    0, // End
                };

                render_context = wglCreateContextAttribsARB(device_context, 0, wgl_context_attrib_list);
                wglMakeCurrent(device_context, render_context);
            }
            else
            {
                MessageBeep(MB_ICONERROR);
                MessageBoxA(0, "System error! Could not initialize OpenGL.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
                return false;
            }
        }
        else
        {
            MessageBeep(MB_ICONERROR);
            MessageBoxA(0, "System error! Could not initialize OpenGL.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
            return false;
        }
    }

    result->Handle = window;
    result->DeviceContext = device_context;
    return true;
}

} // namespace win32


MAIN_WINDOW_CALLBACK(window_callback)
{
    LRESULT result = {};

    switch (message)
    {
        case WM_SIZE:
        {
            current_client_width  = LOWORD(lParam);
            current_client_height = HIWORD(lParam);
            game_loop.viewport_changed = true;
        }
        break;

        case WM_CLOSE:
            game_loop.is_running = false;
        break;

        case WM_DESTROY:
            game_loop.is_running = false;
        break;

        case WM_MOVE:
        case WM_ACTIVATEAPP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        break;

        default:
            result = DefWindowProcA(window, message, wParam, lParam);
    }

    return result;
}

uint32 vk_to_button_id[] =
{
#include "win32_vk_to_button.inl"
};

void process_pending_messages(input_state *input)
{
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT) game_loop.is_running = false;
        TranslateMessage(&message);

        switch (message.message)
        {
            case WM_MOUSEMOVE:
                break;

            case WM_MOUSEWHEEL: input->keyboard_and_mouse.scroll += GET_WHEEL_DELTA_WPARAM(message.wParam);
                break;

            case WM_LBUTTONDOWN: process_button_state(&input->mouse[MOUSE_LEFT], true);
                break;
            case WM_LBUTTONUP: process_button_state(&input->mouse[MOUSE_LEFT], false);
                break;
            case WM_MBUTTONDOWN: process_button_state(&input->mouse[MOUSE_MIDDLE], true);
                break;
            case WM_MBUTTONUP: process_button_state(&input->mouse[MOUSE_MIDDLE], false);
                break;
            case WM_RBUTTONDOWN: process_button_state(&input->mouse[MOUSE_RIGHT], true);
                break;
            case WM_RBUTTONUP: process_button_state(&input->mouse[MOUSE_RIGHT], false);
                break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32 virtual_key_code  = (uint32) message.wParam;
                bool32 was_down = (message.lParam & (1 << 30)) != 0;
                bool32 is_down  = (message.lParam & (1 << 31)) == 0;

                process_button_state(&input->keyboard_and_mouse.buttons[vk_to_button_id[virtual_key_code]], is_down);
            }
            break;

            default:
            {
                DispatchMessageA(&message);
            }
        }
    }
}

struct game_dll
{
    win32::dll dll;
    initialize_memory_t *initialize_memory;
    update_and_render_t *update_and_render;
};

game_dll load_game_dll(string_view dll_path, string_view temp_dll_path, string_view lock_filename)
{
    game_dll result = {};

    uint32 dwAttrib = GetFileAttributes(lock_filename.data);
    bool lock_file_exists = (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));

    if (!lock_file_exists)
    {
        CopyFile(dll_path.data, temp_dll_path.data, FALSE);

        result.dll = win32::dll::load(temp_dll_path.data);

        if (result.dll.handle)
        {
            result.initialize_memory = (initialize_memory_t *) result.dll.get_function("initialize_memory");
            result.update_and_render = (update_and_render_t *) result.dll.get_function("update_and_render");
        }
    }

    return result;
}

void unload_game_dll(game_dll *library)
{
    library->dll.unload();
    library->initialize_memory = NULL;
    library->update_and_render = NULL;
}


int32 WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int32 show_code)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // ======================================================================

    win32::window window;
    win32::create_window(instance, 1600, 900, window_callback, &window);

    initialize_opengl();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    wglSwapIntervalEXT(1);

    // ======================================================================

    game_loop__initialize(&game_loop, (platform::window *) &window);

    auto view = matrix4::identity();
    auto projection = make_projection_matrix_fov(to_radians(60), game_loop.ctx.aspect_ratio, game_loop.ctx.near_clip_dist, game_loop.ctx.far_clip_dist);
    auto projection_ui = matrix4::identity();
    auto ui_framebuffer = create_framebuffer(current_client_width, current_client_height);

    // ======================================================================

    // auto xinput = win32::xinput::load();

    // ======================================================================

#if DLL_BUILD
    char cwd[256] = {};
    uint32 program_path_size = win32::get_program_path(instance, cwd, ARRAY_COUNT(cwd));
    {
        uint32 last_slash_index = 0;
        for (uint32 char_index = 0; char_index < program_path_size; char_index++) {
            if (cwd[char_index] == '\\') {
                last_slash_index = char_index;
            }
        }
        program_path_size = last_slash_index + 1;
    }

    char game_dll_buffer[256] = {};
    memcpy(game_dll_buffer, cwd, program_path_size);
    memcpy(game_dll_buffer + program_path_size, "game.dll", 8);

    char temp_dll_buffer[256] = {};
    memcpy(temp_dll_buffer, cwd, program_path_size);
    memcpy(temp_dll_buffer + program_path_size, "temp.dll", 8);

    char lock_tmp_buffer[256] = {};
    memcpy(lock_tmp_buffer, cwd, program_path_size);
    memcpy(lock_tmp_buffer + program_path_size, "lock.tmp", 8);

    game_dll game = load_game_dll(string_view::from(game_dll_buffer),
                                  string_view::from(temp_dll_buffer),
                                  string_view::from(lock_tmp_buffer));
#else
    game_dll game;
    game.initialize_memory = initialize_memory;
    game.update_and_render = update_and_render;
#endif // DLL_BUILD

    if (game.initialize_memory)
    {
        game.initialize_memory(&game_loop.ctx, game_loop.game_memory);
    }
    else
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not load Game DLL.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
    }

    // ======================================================================

    int32 game_update_frequency_hz = platform::get_monitor_refresh_rate_hz((platform::window *) &window);
    float32 target_seconds_per_frame = 1.0f / game_update_frequency_hz;
    float32 last_frame_dt = target_seconds_per_frame;
    timepoint last_timepoint = platform::clock::now();

    GL_CHECK_ERRORS();

    game_loop.frame_counter = 0;
    game_loop.is_running = true;
    while (game_loop.is_running)
    {
#if DLL_BUILD
        uint64 dll_file_time = win32::get_file_time(game_dll_buffer);
        if (dll_file_time > game.dll.timestamp)
        {
            unload_game_dll(&game);
            game = load_game_dll(string_view::from(game_dll_buffer),
                                 string_view::from(temp_dll_buffer),
                                 string_view::from(lock_tmp_buffer));
        }
#endif // DLL_BUILD

        reset_transitions(game_loop.input.keyboard_and_mouse.buttons, Button_Count);
        game_loop.input.keyboard_and_mouse.scroll = 0;
        process_pending_messages(&game_loop.input);
        // for (int i = 0; i < 4; i++)
        //     xinput.process_gamepad_state(game_loop.input.gamepads + i, i);

        game_loop.input.dt   = last_frame_dt;
        game_loop.input.time = last_timepoint;

        game_loop__update_viewport(&game_loop);
        projection_ui = matrix4::translate(-1, 1, 0)
                      * matrix4::scale(2.0f/game_loop.ctx.viewport.width, -2.0f/game_loop.ctx.viewport.height, 1);

        GL_CHECK_ERRORS();

        if (game.update_and_render)
        {
            game.update_and_render(&game_loop.ctx, game_loop.game_memory, &game_loop.input);
        }

        for (auto cmd : game_loop.ctx.exec_commands)
        {
            if (cmd.tag == ExecutionCommand_ExitGame) game_loop.is_running = false;
#if DEBUG
            if (cmd.tag == ExecutionCommand_DebugDraw_Off) game_loop.is_debug_graph_fps_active = false;
            if (cmd.tag == ExecutionCommand_DebugDraw_Fps) TOGGLE(game_loop.is_debug_graph_fps_active);
#endif // DEBUG
        }
        game_loop.ctx.exec_commands.clear();

        GL_CHECK_ERRORS();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        GL_CHECK_ERRORS();

        for (auto cmd : game_loop.ctx.rend_commands)
        {
            if (cmd.tag == RenderCommand_SetupCamera)
            {
                view = make_lookat_matrix(cmd.position, cmd.position + cmd.forward, cmd.up);
            }
            else if (cmd.tag == RenderCommand_RenderSquare)
            {
                glUseProgram(game_loop.shader_color.id);

                game_loop.shader_color.uniform("u_model", cmd.model);
                game_loop.shader_color.uniform("u_view", view);
                game_loop.shader_color.uniform("u_projection", projection);
                game_loop.shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(game_loop.mesh_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, game_loop.mesh_square.ibo);
                glDrawElements(GL_TRIANGLES, game_loop.mesh_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderCube)
            {
                shader *s = NULL;
                if (cmd.shader_tag == RenderShader_SingleColor) s = &game_loop.shader_color;
                if (cmd.shader_tag == RenderShader_Ground) s = &game_loop.shader_ground;
                if (cmd.shader_tag == RenderShader_Phong) s = &game_loop.shader_phong;
                if (cmd.shader_tag == RenderShader_Sun) s = &game_loop.shader_sun;

                glUseProgram(s->id);

                s->uniform("u_model", cmd.model);
                s->uniform("u_view", view);
                s->uniform("u_projection", projection);
                s->uniform("u_color", cmd.color);

                glBindVertexArray(game_loop.mesh_cube.vao);
                glDrawArrays(GL_TRIANGLES, 0, game_loop.mesh_cube.count);
            }
            else if (cmd.tag == RenderCommand_RenderMesh)
            {
                gpu_mesh *mesh = NULL;
                if (cmd.mesh_tag == RenderMesh_Square) mesh = &game_loop.mesh_square;
                if (cmd.mesh_tag == RenderMesh_Cube) mesh = &game_loop.mesh_cube;
                if (cmd.mesh_tag == RenderMesh_Sphere) mesh = &game_loop.mesh_sphere;

                shader *shader = NULL;
                if (cmd.shader_tag == RenderShader_SingleColor) shader = &game_loop.shader_color;
                if (cmd.shader_tag == RenderShader_Ground) shader = &game_loop.shader_ground;
                if (cmd.shader_tag == RenderShader_Phong) shader = &game_loop.shader_phong;
                if (cmd.shader_tag == RenderShader_Sun) shader = &game_loop.shader_sun;

                if (mesh && shader)
                {
                    auto tm = transform::translate(cmd.position) *
                              transform::scale(cmd.scale);

                    tm.sx = apply_unit_quaternion(cmd.orientation, tm.sx);
                    tm.sy = apply_unit_quaternion(cmd.orientation, tm.sy);
                    tm.sz = apply_unit_quaternion(cmd.orientation, tm.sz);

                    glUseProgram(shader->id);

                    shader->uniform("u_model", to_matrix4(tm));
                    shader->uniform("u_view", view);
                    shader->uniform("u_projection", projection);
                    shader->uniform("u_color", cmd.color);

                    glBindVertexArray(mesh->vao);
                    if (mesh->ibo)
                    {
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
                        glDrawElements(GL_TRIANGLES, mesh->count, GL_UNSIGNED_INT, NULL);
                    }
                    else
                    {
                        glDrawArrays(GL_TRIANGLES, 0, mesh->count);
                    }
                }
            }
            else if (cmd.tag == RenderCommand_RenderSphere)
            {
                auto tm = transform::translate(cmd.position) *
                          transform::scale(cmd.scale);

                tm.sx = apply_unit_quaternion(cmd.orientation, tm.sx);
                tm.sy = apply_unit_quaternion(cmd.orientation, tm.sy);
                tm.sz = apply_unit_quaternion(cmd.orientation, tm.sz);

                // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                draw_platonic_solid(
                    game_loop.mesh_sphere,
                    game_loop.shader_phong,
                    cmd.color,
                    to_matrix4(tm), view, projection);
                // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else if (cmd.tag == RenderCommand_RenderUi)
            {
                ASSERT_FAIL();
            }
        }
        game_loop.ctx.rend_commands.clear();

        GL_CHECK_ERRORS();

        glBindFramebuffer(GL_FRAMEBUFFER, ui_framebuffer.framebuffer_id);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        GL_CHECK_ERRORS();

        for (auto cmd : game_loop.ctx.rend_commands_ui)
        {
            if (cmd.tag == RenderCommand_RenderUi)
            {
                glUseProgram(game_loop.shader_color.id);

                game_loop.shader_color.uniform("u_model", cmd.model);
                game_loop.shader_color.uniform("u_view", matrix4::identity());
                game_loop.shader_color.uniform("u_projection", projection_ui);
                game_loop.shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(game_loop.mesh_square.vao);
                glDrawElements(GL_TRIANGLES, game_loop.mesh_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderBanner)
            {
                auto pNDC = projection * view * matrix4::translate(cmd.position) * V4(0, 0, 0, 1);
                pNDC /= pNDC.w;
                auto pUI = matrix4::scale(0.5f * game_loop.ctx.viewport.width, - 0.5f * game_loop.ctx.viewport.height, 1)
                         * matrix4::translate(1, -1, 0)
                         * pNDC;
                auto m = matrix4::translate(pUI.xyz) * cmd.model;
                glUseProgram(game_loop.shader_color.id);

                game_loop.shader_color.uniform("u_model", m);
                game_loop.shader_color.uniform("u_view", matrix4::identity());
                game_loop.shader_color.uniform("u_projection", projection_ui);
                game_loop.shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(game_loop.mesh_square.vao);
                glDrawElements(GL_TRIANGLES, game_loop.mesh_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderText)
            {
                glUseProgram(game_loop.shader_text.id);
                game_loop.shader_text.uniform("u_model", cmd.model);
                game_loop.shader_text.uniform("u_projection", projection_ui);
                game_loop.shader_text.uniform("u_color", cmd.color);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, game_loop.font_texture.id);

                float32 posx = 0.f;
                float32 posy = 0.f;
                uint32 count = 0;

                string_view strview = string_view::from(cmd.cstr);
                auto temp_memory = ALLOCATE_ALIGNED_BUFFER(game_loop.ctx.temporary_allocator, strview.size * 24 * sizeof(float32), alignof(float32));
                ASSERT(temp_memory.data);
                auto seri_buffer = serializer::from(temp_memory.data, temp_memory.size);

                char c = 0;
                for (char const *str = cmd.cstr; (c = *str) != 0; str++)
                {
                    glyph g = get_glyph(c);

                    float32 px = (float32) posx - g.origin_x;
                    float32 py = (float32) posy - g.origin_y;
                    float32 w  = (float32) g.width;
                    float32 h  = (float32) g.height;

                    float32 uv_x = (float32) g.x / font_14x26.width;
                    float32 uv_y = (float32) g.y / font_14x26.height;
                    float32 uv_x1 = (float32) (g.x + g.width) / font_14x26.width;
                    float32 uv_y1 = (float32) (g.y + g.height) / font_14x26.height;

                    float32 vbo_data[] = {
                         px,     py,       uv_x,  uv_y,
                         px + w, py,       uv_x1, uv_y,
                         px    , py + h,   uv_x,  uv_y1,

                         px + w, py,       uv_x1, uv_y,
                         px + w, py + h,   uv_x1, uv_y1,
                         px,     py + h,   uv_x,  uv_y1,
                    };

                    seri_buffer.push(vbo_data, sizeof(vbo_data));
                    posx += g.width;
                    count += 6;
                }

                glBindVertexArray(game_loop.mesh_square_uv.vao);

                glBindBuffer(GL_ARRAY_BUFFER, game_loop.mesh_square_uv.vbo);
                glBufferData(GL_ARRAY_BUFFER, seri_buffer.size, seri_buffer.data, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glDrawArrays(GL_TRIANGLES, 0, count);
            }
            else
            {
                ASSERT_FAIL();
            }
        }
        game_loop.ctx.rend_commands_ui.clear();

        GL_CHECK_ERRORS();

        // Draw UI on top of everything
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(game_loop.shader_framebuffer.id);
            game_loop.shader_framebuffer.uniform("u_framebuffer", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ui_framebuffer.color_texture_id);

            glBindVertexArray(game_loop.mesh_square.vao);
            glDrawElements(GL_TRIANGLES, game_loop.mesh_square.count, GL_UNSIGNED_INT, NULL);

            glEnable(GL_DEPTH_TEST);
        }

        GL_CHECK_ERRORS();

#if DEBUG
        if (game_loop.is_debug_graph_fps_active)
        {
            game_loop.debug_graph_fps.graph[(game_loop.debug_graph_fps.index++) % game_loop.debug_graph_fps.count] = game_loop.input.dt * 1000.f;
            draw_debug_graph(&game_loop.debug_graph_fps, &game_loop.mesh_square, &game_loop.shader_color);
        }
#endif // DEBUG

        SwapBuffers(window.DeviceContext);

        game_loop.ctx.temporary_allocator.reset();
        game_loop.frame_counter += 1;

        timepoint end_of_frame = platform::clock::now();
        last_frame_dt = (float32) get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    return 0;
}


#include <memory/allocator.cpp>
#include <os/platform_win32.cpp>

#include "game_loop.cpp"
#include "debug_graph.cpp"

#if DLL_BUILD
#include <memory_bucket.cpp>
#include <image/png.cpp>
#include <crc.cpp>
#else
#include <game/game.cpp>
#include <context.cpp>
#include <string_id.cpp>
#include <collision.cpp>
#include <ecs/entity_manager.cpp>
#include <ecs/archetype.cpp>
#endif // DLL_BUILD
#include <platonic_solids.cpp>

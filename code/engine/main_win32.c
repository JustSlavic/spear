#include <corelibs/base.h>
#include <corelibs/platform.h>
#include <engine/engine.h>

#include <stdio.h>
#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>

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

typedef const char *WINAPI wglGetExtensionsStringARBType(HDC hdc);
typedef BOOL WINAPI wglChoosePixelFormatARBType(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC WINAPI wglCreateContextAttribsARBType(HDC hdc, HGLRC hShareContext, int const *attribList);
typedef BOOL wglSwapIntervalEXTType(int interval);
typedef int wglGetSwapIntervalEXTType(void);

static wglGetExtensionsStringARBType *wglGetExtensionsStringARB;
static wglChoosePixelFormatARBType *wglChoosePixelFormatARB;
static wglCreateContextAttribsARBType *wglCreateContextAttribsARB;
static wglSwapIntervalEXTType *wglSwapIntervalEXT;
static wglGetSwapIntervalEXTType *wglGetSwapIntervalEXT;

#define GL_DEPTH_STENCIL                  0x84F9
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81

/* ClearBufferMask */
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define GL_FALSE                          0
#define GL_TRUE                           1

/* DataType */
#define GL_FLOAT                          0x1406

typedef char GLchar;
typedef long long GLsizeiptr;

typedef void glGenBuffersType(GLsizei n, GLuint *buffers);
typedef void glBindBufferType(GLenum target, GLuint buffer);
typedef void glBufferDataType(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void glGenVertexArraysType(GLsizei n, GLuint *arrays);
typedef void glBindVertexArrayType(GLuint array);
typedef void glVertexAttribPointerType(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void glEnableVertexAttribArrayType(GLuint index);
typedef GLuint glCreateShaderType(GLenum shaderType);
typedef void glShaderSourceType(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void glCompileShaderType(GLuint shader);
typedef GLuint glCreateProgramType(void);
typedef void glAttachShaderType(GLuint program, GLuint shader);
typedef void glDetachShaderType(GLuint program, GLuint shader);
typedef void glLinkProgramType(GLuint program);
typedef void glUseProgramType(GLuint program);
typedef void glGetShaderivType(GLuint shader, GLenum pname, GLint *params);

static glGenBuffersType *glGenBuffers;
static glBindBufferType *glBindBuffer;
static glBufferDataType *glBufferData;
static glGenVertexArraysType *glGenVertexArrays;
static glBindVertexArrayType *glBindVertexArray;
static glVertexAttribPointerType *glVertexAttribPointer;
static glEnableVertexAttribArrayType *glEnableVertexAttribArray;
static glCreateShaderType *glCreateShader;
static glShaderSourceType *glShaderSource;
static glCompileShaderType *glCompileShader;
static glCreateProgramType *glCreateProgram;
static glAttachShaderType *glAttachShader;
static glDetachShaderType *glDetachShader;
static glLinkProgramType *glLinkProgram;
static glUseProgramType *glUseProgram;
static glGetShaderivType *glGetShaderiv;

bool initialize_opengl(void)
{
#define WGL_GET_PROC_ADDRESS(NAME) NAME = (NAME##Type *) wglGetProcAddress(STRINGIFY(NAME)); \
    if (NAME == NULL) return false;

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

#undef WGL_GET_PROC_ADDRESS
    return true;
}

int get_width(RECT rectangle)
{
    int result = rectangle.right - rectangle.left;
    return result;
}

int get_height(RECT rectangle)
{
    int result = rectangle.bottom - rectangle.top;
    return result;
}

typedef struct window
{
    HWND Handle;
    HDC DeviceContext;
} window;

#define MAIN_WINDOW_CALLBACK(NAME) LRESULT CALLBACK NAME(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
typedef MAIN_WINDOW_CALLBACK(MainWindowCallbackType);

static engine g_engine;
static bool is_running;
static uint32 current_client_width;
static uint32 current_client_height;

window create_window(HINSTANCE Instance, int ClientWidth, int ClientHeight, MainWindowCallbackType *WindowCallback)
{
    int PrimaryMonitorWidth  = GetSystemMetrics(SM_CXSCREEN);
    int PrimaryMonitorHeight = GetSystemMetrics(SM_CYSCREEN);

    HBRUSH BlackBrush = CreateSolidBrush(RGB(0, 0, 0));

    WNDCLASSA WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "WindowClass";
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = BlackBrush;

    ATOM WindowClassAtom = RegisterClassA(&WindowClass);
    RECT WindowRectangle = { 0, 0, (int) ClientWidth, (int) ClientHeight };
    AdjustWindowRect(&WindowRectangle, WS_OVERLAPPEDWINDOW, false);

    HWND WindowHandle = CreateWindowExA(0, WindowClass.lpszClassName, "OpenGL temp window", WS_OVERLAPPEDWINDOW, 0, 0, 50, 50, 0, 0, Instance, NULL);

    HDC DeviceContext = GetDC(WindowHandle);
    HGLRC TempRenderContext = {};
    HGLRC RenderContext = {};

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

    int suggested_pixel_format_index = ChoosePixelFormat(DeviceContext, &desired_pixel_format);
    PIXELFORMATDESCRIPTOR suggested_pixel_format;
    DescribePixelFormat(DeviceContext, suggested_pixel_format_index, sizeof(suggested_pixel_format), &suggested_pixel_format);

    if (SetPixelFormat(DeviceContext, suggested_pixel_format_index, &suggested_pixel_format))
    {
        TempRenderContext = wglCreateContext(DeviceContext);
        if (wglMakeCurrent(DeviceContext, TempRenderContext))
        {
            wglGetExtensionsStringARB = (wglGetExtensionsStringARBType *) wglGetProcAddress("wglGetExtensionsStringARB");
            wglChoosePixelFormatARB = (wglChoosePixelFormatARBType *) wglGetProcAddress("wglChoosePixelFormatARB");
            wglCreateContextAttribsARB = (wglCreateContextAttribsARBType *) wglGetProcAddress("wglCreateContextAttribsARB");
            // @todo Check if 'WGL_EXT_swap_control' extension is available
            wglSwapIntervalEXT = (wglSwapIntervalEXTType *) wglGetProcAddress("wglSwapIntervalEXT");
            wglGetSwapIntervalEXT = (wglGetSwapIntervalEXTType *) wglGetProcAddress("wglGetSwapIntervalEXT");

            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(TempRenderContext);
            ReleaseDC(WindowHandle, DeviceContext);
            DestroyWindow(WindowHandle);

            WindowHandle = CreateWindowExA(
                0, WindowClass.lpszClassName,
                "window",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                (PrimaryMonitorWidth - get_width(WindowRectangle)) / 2,
                (PrimaryMonitorHeight - get_height(WindowRectangle)) / 2,
                get_width(WindowRectangle),
                get_height(WindowRectangle),
                0, 0, Instance, NULL);

            DeviceContext = GetDC(WindowHandle);

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

            int pixel_format;
            uint32_t number_formats;
            wglChoosePixelFormatARB(DeviceContext, wgl_attribute_list, NULL, 1, &pixel_format, &number_formats);
            SetPixelFormat(DeviceContext, pixel_format, &desired_pixel_format);

            int wgl_context_attrib_list[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 0,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0, // End
            };

            RenderContext = wglCreateContextAttribsARB(DeviceContext, 0, wgl_context_attrib_list);
            wglMakeCurrent(DeviceContext, RenderContext);
        }
    }

    window Result;
    Result.Handle = WindowHandle;
    Result.DeviceContext = DeviceContext;
    return Result;
}

MAIN_WINDOW_CALLBACK(window_callback)
{
    LRESULT result;
    memset(&result, 0, sizeof(result));

    switch (message)
    {
        case WM_SIZE:
            current_client_width  = LOWORD(lParam);
            current_client_height = HIWORD(lParam);
        break;

        case WM_CLOSE:
        case WM_DESTROY:
            is_running = false;
        break;

        default:
            result = DefWindowProcA(window, message, wParam, lParam);
    }

    return result;
}

void process_pending_messages()
{
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT) is_running = false;
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

char const *vertex_shader_text = "#version 410\n"
"layout (location = 0) in vec2 vertex_position;\n"
"layout (location = 1) in vec3 vertex_color;\n"
"out vec4 fragment_color;\n"
"void main()\n"
"{\n"
"    fragment_color = vec4(vertex_color, 1.0);\n"
"    gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);\n"
"}\n";

char const *fragment_shader_text =
"#version 410\n"
"in vec4 fragment_color;\n"
"out vec4 result_color;\n"
"void main()\n"
"{\n"
"    result_color = fragment_color;\n"
"}\n";

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCode)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    spear_engine_init(&g_engine);


    window w = create_window(Instance, 1600, 900, window_callback);
    initialize_opengl();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    wglSwapIntervalEXT(1);

    uint32_t vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, (char const **) &vertex_shader_text, (int const *) NULL);
    glCompileShader(vertex_shader_id);
    int successful;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &successful);

    uint32_t fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, (char const **) &fragment_shader_text, (int const *) NULL);
    glCompileShader(fragment_shader_id);
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &successful);

    int shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader_id);
    glAttachShader(shader_id, fragment_shader_id);
    glLinkProgram(shader_id);

    float vertices[] =
    {
        // positions   // colors
        -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f
    };

    uint32_t vao_id, vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));

    is_running = true;
    while (is_running)
    {
        process_pending_messages();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_id);
        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SwapBuffers(w.DeviceContext);
    }

    return 0;
}

#include <corelibs/base.c>
#include <corelibs/math.c>
#include <corelibs/platform_win32.c>
#include <corelibs/memory/allocator.c>

#include <gamelibs/input.c>

#include <engine/engine.c>
#include <engine/graphics/render.c>

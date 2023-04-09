#ifndef GFX_GL_WIN32_HPP
#define GFX_GL_WIN32_HPP

#include <base.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


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

typedef void glActiveTextureType(GLenum texture);
GLOBAL glActiveTextureType *glActiveTexture;


namespace gfx::gl {

bool32 create_opengl_window(HINSTANCE Instance, int32 ClientWidth, int32 ClientHeight, MainWindowCallbackType *WindowCallback, void *w)
{
    auto *win32_window = (win32::window *) w;

    int32 PrimaryMonitorWidth  = GetSystemMetrics(SM_CXSCREEN);
    int32 PrimaryMonitorHeight = GetSystemMetrics(SM_CYSCREEN);

    HBRUSH BlackBrush = CreateSolidBrush(RGB(0, 0, 0));

    WNDCLASSA WindowClass {};
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

    RECT WindowRectangle = { 0, 0, ClientWidth, ClientHeight };
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
                    (PrimaryMonitorWidth - win32::width(WindowRectangle)) / 2,
                    (PrimaryMonitorHeight - win32::height(WindowRectangle)) / 2,
                    win32::width(WindowRectangle),
                    win32::height(WindowRectangle),
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
                    // WGL_SAMPLE_BUFFERS_ARB, 1,
                    // WGL_SAMPLES_ARB, 1,
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

    // glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    win32_window->handle = window;
    win32_window->device_context = device_context;
    return true;
}

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

void vsync(void *window, bool turn_on)
{
    wglSwapIntervalEXT(turn_on ? 1 : 0);
}

void swap_buffers(void *window)
{
    auto *win32_window = (win32::window *) window;
    SwapBuffers(win32_window->device_context);
}

void destroy_window_and_driver(void *window, void *driver)
{
}


} // namespace gfx::gl

#endif // GFX_GL_WIN32_HPP

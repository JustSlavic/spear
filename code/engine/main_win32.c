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

static spear_engine g_engine;
static spear_input g_input;
static uint32 g_client_width;
static uint32 g_client_height;

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
    if (!WindowClassAtom)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not register window class.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return (window) {};
    }

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
            g_client_width = LOWORD(lParam);
            g_client_height = HIWORD(lParam);
            g_engine.viewport_changed = true;
        break;

        case WM_CLOSE:
        case WM_DESTROY:
            g_engine.running = false;
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

void process_pending_messages(spear_input *input)
{
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT) g_engine.running = false;
        TranslateMessage(&message);

        switch (message.message)
        {
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32 virtual_key_code  = (uint32) message.wParam;
                bool32 was_down = (message.lParam & (1 << 30)) != 0;
                bool32 is_down  = (message.lParam & (1 << 31)) == 0;

                input_button_update(&input->keyboard_and_mouse.buttons[vk_to_button_id[virtual_key_code]], is_down);
            }
            break;

            default:
            {
                DispatchMessageA(&message);
            }
        }

    }
}

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCode)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    spear_engine_init(&g_engine);
    window w = create_window(Instance, g_engine.current_client_width, g_engine.current_client_height, window_callback);
    spear_engine_init_graphics(&g_engine);
    spear_engine_create_meshes(&g_engine);
    spear_engine_compile_shaders(&g_engine);
    spear_engine_load_game_dll(&g_engine, "spear_game.dll");
    spear_engine_game_init(&g_engine);

    duration last_frame_dt = duration_create_milliseconds(16);
    timepoint last_timepoint = platform_clock_now();

    g_engine.running = true;
    while (g_engine.running)
    {
        memory_allocator_arena_reset(g_engine.temporary);
        spear_engine_load_game_dll(&g_engine, "spear_game.dll");

        input_button_reset_transitions(g_input.keyboard_and_mouse.buttons, Keyboard_Button_Count);
        g_input.keyboard_and_mouse.mouse_scroll = 0;

        process_pending_messages(&g_input);
        // Set mouse pos
        {
            POINT pos;
            GetCursorPos(&pos);
            ScreenToClient(w.Handle, &pos);
            spear_engine_input_mouse_pos_set(&g_engine, &g_input, pos.x, pos.y);
        }

        float64 dt = duration_get_seconds(last_frame_dt);
        g_input.dt = dt;
        g_input.time = timepoint_get_seconds(last_timepoint);

        if (g_engine.viewport_changed)
        {
            spear_engine_update_viewport(&g_engine, g_client_width, g_client_height);
        }

        spear_engine_game_update(&g_engine, &g_input, dt);
        spear_engine_game_render(&g_engine);

        SwapBuffers(w.DeviceContext);

        timepoint end_of_frame = platform_clock_now();
        last_frame_dt = get_duration_between_timepoints(last_timepoint, end_of_frame);
        last_timepoint = end_of_frame;
    }

    return 0;
}

#include <corelibs/math.c>
#include <corelibs/collision.c>
#include <corelibs/platform_win32.c>
#include <corelibs/memory/allocator.c>
#include <corelibs/parse_primitives.c>
#include <corelibs/file_formats/bmp.c>
#include <corelibs/file_formats/wav.c>
#include <corelibs/file_formats/obj.c>

#include <gamelibs/input.c>

#include <engine/engine.c>
#include <engine/graphics/render.c>
#include <engine/graphics/opengl_win32.c>
#include <engine/primitive_meshes.c>
#include <engine/audio/audio.c>
#include <engine/audio/audio_dsound.c>

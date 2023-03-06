// Windows
#include <platform_win32.hpp>

// Project headers
#include <base.hpp>
#include <string.hpp>
#include <math/vector3.hpp>
#include <renderer.hpp>
#include <opengl/gl.hpp>
#include <game.hpp>


GLOBAL bool32_t running;

GLOBAL uint32_t current_client_width;
GLOBAL uint32_t current_client_height;
GLOBAL bool32_t viewport_changed;


struct game_dll
{
    win32::dll dll;
    initialize_memory_t *initialize_memory;
    update_and_render_t *update_and_render;
};


game_dll load_game_dll(string dll_path, string temp_dll_path, string lock_filename)
{
    game_dll result = {};

    uint32_t dwAttrib = GetFileAttributes(lock_filename.data);
    bool lock_file_exists = (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));

    if (!lock_file_exists)
    {
        CopyFile(dll_path.data, temp_dll_path.data, FALSE);

        result.dll = win32::load_dll(temp_dll_path.data);

        if (result.dll)
        {
            result.initialize_memory = result.dll.get_function<initialize_memory_t *>("initialize_memory");
            result.update_and_render = result.dll.get_function<update_and_render_t *>("update_and_render");
        }
    }

    return result;
}


void unload_game_dll(game_dll *library)
{
    win32::unload_dll(&library->dll);
    library->initialize_memory = NULL;
    library->update_and_render = NULL;
}


LRESULT CALLBACK main_window_callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = {};

    switch (message)
    {
        case WM_SIZE:
        {
            current_client_width  = LOWORD(lParam);
            current_client_height = HIWORD(lParam);
            viewport_changed = true;
        }
        break;

        case WM_MOVE:
        {
        }
        break;

        case WM_CLOSE:
        {
            running = false;
        }
        break;

        case WM_DESTROY:
        {
            running = false;
        }
        break;

        case WM_ACTIVATEAPP:
        {
        }
        break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
        }
        break;

        default:
        {
            result = DefWindowProcA(window, message, wParam, lParam);
        }
    }

    return result;
}


void process_pending_messages()
{
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT) running = false;
        TranslateMessage(&message);

        switch (message.message)
        {
            case WM_MOUSEMOVE:
            {
            }
            break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
            }
            break;

            default:
            {
                DispatchMessageA(&message);
            }
        }
    }
}


int32_t WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int32_t show_code)
{
    int32_t primary_monitor_width  = GetSystemMetrics(SM_CXSCREEN);
    int32_t primary_monitor_height = GetSystemMetrics(SM_CYSCREEN);
    HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0));

    WNDCLASSA window_class {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = main_window_callback;
    window_class.hInstance = instance;
    window_class.lpszClassName = "window_class";
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = black_brush;

    ATOM register_class_result = RegisterClassA(&window_class);
    if (!register_class_result)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not register window class.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
    }

    HWND window = CreateWindowExA(0, window_class.lpszClassName, "OpenGL temp window",
        WS_OVERLAPPEDWINDOW, 0, 0, 50, 50, 0, 0, instance, NULL);

    if (!window)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not create a window.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
    }

    int32_t client_width  = 800;
    int32_t client_height = 600;
    RECT window_rectangle = { 0, 0, client_width, client_height };
    if (!AdjustWindowRect(&window_rectangle, WS_OVERLAPPEDWINDOW, false))
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! AdjustWindowRect failed.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
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
        desired_pixel_format.cColorBits = 32;
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
                    0, window_class.lpszClassName,
                    "Window",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    (primary_monitor_width - win32::width(window_rectangle)) / 2,
                    (primary_monitor_height - win32::height(window_rectangle)) / 2,
                    win32::width(window_rectangle),
                    win32::height(window_rectangle),
                    0, 0, instance, NULL);

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
                    // WGL_SAMPLES_ARB, 4,
                    0, // End
                };

                int32_t pixel_format;
                uint32_t number_formats;
                wglChoosePixelFormatARB(device_context, wgl_attribute_list, NULL, 1, &pixel_format, &number_formats);
                SetPixelFormat(device_context, pixel_format, &desired_pixel_format);

                int32_t wgl_context_attrib_list[] =
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
                return 1;
            }
        }
        else
        {
            MessageBeep(MB_ICONERROR);
            MessageBoxA(0, "System error! Could not initialize OpenGL.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
            return 1;
        }
    }

    gfx::initialize(gfx::graphics_api::opengl);
    gfx::set_clear_color(0, 0, 0, 1);
    gfx::vsync(true);

    char buffer[256] = {};

    uint32_t program_path_size = win32::get_program_path(instance, buffer, ARRAY_COUNT(buffer));

    uint32_t last_slash_index = 0;
    for (uint32_t char_index = 0; char_index < program_path_size; char_index++) {
        if (buffer[char_index] == '\\') {
            last_slash_index = char_index;
        }
    }
    uint32_t program_path_size_no_filename = last_slash_index + 1;

    char game_dll_buffer[256] = {};
    memory::copy(game_dll_buffer, buffer, program_path_size_no_filename);
    memory::copy(game_dll_buffer + program_path_size_no_filename, "game.dll", 8);

    char temp_dll_buffer[256] = {};
    memory::copy(temp_dll_buffer, buffer, program_path_size_no_filename);
    memory::copy(temp_dll_buffer + program_path_size_no_filename, "temp.dll", 8);

    char lock_tmp_buffer[256] = {};
    memory::copy(lock_tmp_buffer, buffer, program_path_size_no_filename);
    memory::copy(lock_tmp_buffer + program_path_size_no_filename, "lock.tmp", 8);

    game_dll game = load_game_dll(game_dll_buffer, temp_dll_buffer, lock_tmp_buffer);
    memory_block game_memory = win32::allocate_memory((void *) TERABYTES(2), MEGABYTES(8));

    execution_context context = {};

    if (game.initialize_memory)
    {
        game.initialize_memory(&context, game_memory);
    }
    else
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not load Game DLL.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
    }

    auto view = math::matrix4::identity();
    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gfx::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    running = true;
    while (running)
    {
        process_pending_messages();

        if (viewport_changed)
        {
            auto viewport = gfx::make_viewport(current_client_width, current_client_height, aspect_ratio);
            gfx::set_viewport(viewport);
            viewport_changed = false;
        }

        gfx::clear();

        if (game.update_and_render)
        {
            game.update_and_render(&context, game_memory);
        }

        for (usize cmd_index = context.next_execution_command_index;
             cmd_index < context.execution_command_queue_size;
             cmd_index = (cmd_index + 1) % ARRAY_COUNT(context.execution_command_queue))
        {
            auto cmd = pop_execution_command(&context);
            switch (cmd.type)
            {
                case execution_command::exit:
                {
                    running = false;
                }
                break;
            }
        }

        for (usize cmd_index = 0; cmd_index < context.render_command_queue_size; cmd_index++)
        {
            auto *cmd = context.render_command_queue + cmd_index;
            switch (cmd->type)
            {
                case gfx::render_command::setup_projection_matrix:
                break;

                case gfx::render_command::setup_camera:
                {
                    view = gfx::make_look_at_matrix(cmd->camera_position, cmd->look_at_position, cmd->camera_up_direction);
                }
                break;

                case gfx::render_command::draw_rectangle:
                {
                    gfx::draw_rectangle(cmd, view, projection);
                }
                break;
            }
        }
        context.render_command_queue_size = 0;

        SwapBuffers(device_context);
    }

    return 0;
}


#include <renderer.cpp>

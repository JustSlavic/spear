// Windows
#include <platform_win32.hpp>

// Project headers
#include <base.hpp>
#include <math/vector3.hpp>
#include <opengl/gl.hpp>


GLOBAL bool32_t running;

GLOBAL uint32_t current_client_width;
GLOBAL uint32_t current_client_height;
GLOBAL bool32_t viewport_changed;


char const *vs_source = R"GLSL(
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

out vec4 fragment_color;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    vec4 p = u_projection * u_view * vec4(vertex_position, 1.0);
    fragment_color = vec4(vertex_color, 1.0);
    gl_Position = p;
}
)GLSL";


char const *fs_source = R"GLSL(
#version 400

in vec4 fragment_color;
out vec4 result_color;

void main()
{
    result_color = fragment_color;
}
)GLSL";


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

    gl::initialize();


    struct vertex
    {
        math::vector3 position;
        math::vector3 color;
    };

    vertex vertices[] =
    {
        { math::make_vector3(-1, -1, 0), math::make_vector3(0, 0, 1) }, // 0 bottom left
        { math::make_vector3( 1, -1, 0), math::make_vector3(0, 1, 0) }, // 1 bottom right
        { math::make_vector3( 1,  1, 0), math::make_vector3(1, 0, 0) }, // 2 top right
        { math::make_vector3(-1,  1, 0), math::make_vector3(1, 1, 0) }, // 3 top left
    };

    uint32_t vertex_buffer_id = 0;
    {
        glGenBuffers(1, &vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    uint32_t indices[] = {
        0, 1, 2,  // first triangle
        2, 3, 0,  // second triangle
    };

    uint32_t index_buffer_id = 0;
    {
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    uint32_t vertex_array_id = 0;
    {
        glGenVertexArrays(1, &vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        uint32_t attrib_index = 0;
        uint64_t offset = 0;
        {
            uint32_t count = 3; // Because it's vector3

            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                sizeof(vertex),    // Stride
                (void *) offset);  // Offset

            attrib_index += 1;
            offset += (count * sizeof(float32));
        }
        {
            uint32_t count = 3; // Because it's color32

            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                sizeof(vertex),    // Stride
                (void *) offset);  // Offset

            attrib_index += 1;
            offset += (count * sizeof(float32));
        }
    }

    auto plane_vs = gl::compile_shader(vs_source, GL_VERTEX_SHADER);
    auto plane_fs = gl::compile_shader(fs_source, GL_FRAGMENT_SHADER);
    auto plane_shader = gl::link_shader(plane_vs, plane_fs);
    glDeleteShader(plane_vs);
    glDeleteShader(plane_fs);
    GL_CHECK_ERRORS();

    auto camera_position = math::make_vector3(0, 0, -3);
    auto look_at_position = math::make_vector3(0, 0, 0);
    auto camera_up_direction = math::make_vector3(0, 1, 0);
    auto view = gl::make_look_at_matrix(camera_position, look_at_position, camera_up_direction);

    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gl::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    running = true;
    while (running)
    {
        process_pending_messages();

        if (viewport_changed)
        {
            auto viewport = gl::make_viewport(current_client_width, current_client_height, aspect_ratio);
            gl::set_viewport(viewport);
            viewport_changed = false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw plane
        {
            glUseProgram(plane_shader.id);

            gl::uniform(plane_shader, "u_view", view);
            gl::uniform(plane_shader, "u_projection", projection);

            glBindVertexArray(vertex_array_id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
            glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indices), GL_UNSIGNED_INT, NULL);
        }

        SwapBuffers(device_context);

        Sleep(30);
    }

    return 0;
}

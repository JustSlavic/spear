#include <base.h>
#include <string_id.hpp>
#include <string_view.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "platform_win32.hpp"

#include "game_interface.hpp"
#include "gfx/renderer.hpp"
#include "gfx/viewport.hpp"
#include "rs/resource_system.hpp"

#include "xinput.hpp"
#include <stdio.h>

GLOBAL bool32 running;
GLOBAL uint32 current_client_width;
GLOBAL uint32 current_client_height;
GLOBAL bool32 viewport_changed;


MAIN_WINDOW_CALLBACK(window_callback)
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

        case WM_CLOSE:
            running = false;
        break;

        case WM_DESTROY:
            running = false;
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


void process_pending_messages(input_state *input)
{
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT) running = false;
        TranslateMessage(&message);

        switch (message.message)
        {
            case WM_MOUSEMOVE:
                break;

            case WM_MOUSEWHEEL: input->mouse.scroll += GET_WHEEL_DELTA_WPARAM(message.wParam);
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

                process_button_state(&input->keyboard[win32::map_button_from_virtual_key_code(virtual_key_code)], is_down);
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

    auto global_memory = win32::allocate_memory((void *) TERABYTES(1), MEGABYTES(100));
    auto global_arena  = memory_allocator::make_arena(global_memory);

    auto game_memory = global_arena.allocate_buffer(MEGABYTES(5));
    auto resource_allocator = global_arena.allocate_arena(MEGABYTES(10));
    auto renderer_allocator = global_arena.allocate_arena(MEGABYTES(10));
    auto string_id_allocator = global_arena.allocate_arena(KILOBYTES(10));
    auto execution_commands_memory = global_arena.allocate_buffer(KILOBYTES(10));
    auto render_commands_memory = global_arena.allocate_buffer(KILOBYTES(10));

    auto temporary_arena = global_arena.allocate_arena(MEGABYTES(50));

    auto string_id_storage = string_id::initialize(string_id_allocator);

    // ======================================================================

    rs::storage rs;
    memset(&rs, 0, sizeof(rs));

    context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.temporary_allocator = temporary_arena;
    ctx.resource_allocator = resource_allocator;
    ctx.renderer_allocator = renderer_allocator;
    ctx.strids = &string_id_storage;
    ctx.rs = &rs;

    // ======================================================================

    win32::window window;
    win32::create_opengl_window(instance, 1600, 900, window_callback, &window);

    gfx::driver driver = gfx::driver::initialize_opengl();

    int32 monitor_refresh_rate_hz = GetDeviceCaps(((win32::window *) &window)->device_context, VREFRESH);
    ctx.screen_width = GetDeviceCaps(((win32::window *) &window)->device_context, HORZRES);
    ctx.screen_height = GetDeviceCaps(((win32::window *) &window)->device_context, VERTRES);
    driver.clear_color(0, 0, 0, 1);

    driver.depth_test(true);
    driver.write_depth(true);
    driver.vsync(true);

    // ======================================================================

    ctx.aspect_ratio = 16.0f / 9.0f;
    ctx.near_clip_dist = 0.05f;
    ctx.near_clip_width = 2 * ctx.near_clip_dist * tanf(0.5f * to_radians(60));
    ctx.near_clip_height = ctx.near_clip_width / ctx.aspect_ratio;
    ctx.far_clip_dist = 100.f;
    ctx.debug_load_file = win32::load_file;

    auto view = matrix4::identity();
    auto projection = driver.make_projection_matrix_fov(to_radians(60), ctx.aspect_ratio, ctx.near_clip_dist, ctx.far_clip_dist);

    // ======================================================================

    auto xinput = win32::xinput::load();

    // ======================================================================

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

    if (game.initialize_memory)
    {
        game.initialize_memory(&ctx, game_memory);
    }
    else
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not load Game DLL.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return 1;
    }

    // ======================================================================

    input_state input = {};

    int32 game_update_frequency_hz = monitor_refresh_rate_hz;
    float32 target_seconds_per_frame = 1.0f / game_update_frequency_hz;
    float32 last_frame_dt = target_seconds_per_frame;
    timepoint last_timepoint = win32::wall_clock::now();

    running = true;
    while (running)
    {
        uint64 dll_file_time = win32::get_file_time(game_dll_buffer);
        if (dll_file_time > game.dll.timestamp)
        {
            unload_game_dll(&game);
            game = load_game_dll(string_view::from(game_dll_buffer),
                                 string_view::from(temp_dll_buffer),
                                 string_view::from(lock_tmp_buffer));
        }

        reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
        reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
        input.mouse.scroll = 0;
        process_pending_messages(&input);
        window.get_mouse_pos(&input.mouse.x, &input.mouse.y);
        for (int i = 0; i < 4; i++)
            xinput.process_gamepad_state(input.gamepads + i, i);

        input.dt   = last_frame_dt;
        input.time = last_timepoint;

        if (viewport_changed)
        {
            auto viewport = gfx::viewport::make(current_client_width, current_client_height, ctx.aspect_ratio);
            driver.set_viewport(viewport);
            viewport_changed = false;

            ctx.window_width = current_client_width;
            ctx.window_height = current_client_height;

            ctx.letterbox_width = viewport.width;
            ctx.letterbox_height = viewport.height;
        }

        if (game.update_and_render)
        {
            game.update_and_render(&ctx, game_memory, &input);
        }

        for (auto cmd : ctx.exec_commands)
        {
            if (cmd.kind == exec_command::exit) running = false;
        }

        driver.clear();

        for (auto cmd : ctx.rend_commands)
        {
            if (cmd.kind == rend_command::setup_camera)
                view = driver.make_look_at_matrix(cmd.position, cmd.position + cmd.forward, cmd.up);
            if (cmd.kind == rend_command::render_mesh_single_color)
                driver.render_mesh_single_color(&ctx, cmd.model, view, projection, cmd.mesh_token, cmd.shader_token, cmd.color);
            if (cmd.kind == rend_command::render_mesh_texture)
                driver.render_mesh_texture(&ctx, cmd.model, view, projection, cmd.mesh_token, cmd.shader_token, cmd.texture_token);
        }
        ctx.rend_commands.clear();

        driver.swap_buffers(&window);

        ctx.temporary_allocator.reset();

        timepoint end_of_frame = win32::wall_clock::now();
        last_frame_dt = (float32) get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    return 0;
}


#include <memory_bucket.cpp>
#include <memory_allocator.cpp>
#include <string_id.cpp>
#include <image/png.cpp>
#include <crc.cpp>

#include "gfx/renderer.cpp"
#include "rs/resource_system.cpp"

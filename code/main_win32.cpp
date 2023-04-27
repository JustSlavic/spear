// Windows
#include <platform_win32.hpp>

// Project headers
#include <base.hpp>
#include <game_interface.hpp>
#include <string.hpp>
#include <string_id.hpp>
#include <math/vector3.hpp>
#include <gfx/renderer.hpp>
#include <os/time.hpp>
#include <input.hpp>
#include <rs/resource.hpp>

#include <stdio.h>

GLOBAL bool32 running;

GLOBAL uint32 current_client_width;
GLOBAL uint32 current_client_height;
GLOBAL bool32 viewport_changed;


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


void process_pending_messages(input_devices *inp)
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

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32 virtual_key_code  = (uint32) message.wParam;
                bool32 alt_down = (message.lParam & (1 << 29)) != 0;
                bool32 was_down = (message.lParam & (1 << 30)) != 0;
                bool32 is_down  = (message.lParam & (1 << 31)) == 0;

                switch (virtual_key_code)
                {
                    case VK_ESCAPE: process_button_state(&inp->keyboard_device[keyboard::esc], is_down);
                        break;
                    case 'W': process_button_state(&inp->keyboard_device[keyboard::w], is_down);
                        break;
                    case 'A': process_button_state(&inp->keyboard_device[keyboard::a], is_down);
                        break;
                    case 'S': process_button_state(&inp->keyboard_device[keyboard::s], is_down);
                        break;
                    case 'D': process_button_state(&inp->keyboard_device[keyboard::d], is_down);
                        break;
                    case 'Y': process_button_state(&inp->keyboard_device[keyboard::y], is_down);
                        break;
                }
            }
            break;

            default:
            {
                DispatchMessageA(&message);
            }
        }
    }
}


int32 WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int32 show_code)
{
    auto choosen_api = gfx::graphics_api::opengl;
    win32::window window = {};
    gfx::driver driver = {};

    if (choosen_api == gfx::graphics_api::opengl)
    {
        win32::create_opengl_window(instance, 1600, 900, window_callback, &window);
        gfx::initialize_opengl(&driver);
    }
    else if (choosen_api == gfx::graphics_api::dx11)
    {
        win32::create_simple_window(instance, 800, 600, window_callback, &window);
        // gfx::initialize_dx11(&window, &driver);
    }

    gfx::set_clear_color(0, 0, 0, 1);
    int32 monitor_refresh_rate_hz = GetDeviceCaps(((win32::window *) &window)->device_context, VREFRESH);
    gfx::vsync(&window, true);

    // @todo: Allocate the memory and initialize allocators on it

    memory_block global_memory = win32::allocate_memory((void *) TERABYTES(1), MEGABYTES(26));

    memory::allocator global_allocator;
    memory::initialize_memory_arena(&global_allocator, global_memory);

    memory_block platform_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block game_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(20));
    memory_block scratchpad_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block renderer_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(2));
    memory_block resource_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block string_id_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));

    memory::allocator platform_allocator = {};
    memory::initialize_memory_arena(&platform_allocator, platform_memory);

    execution_context context = {};
    memory::initialize_memory_arena(&context.temporary_allocator, scratchpad_memory);
    memory::initialize_memory_arena(&context.renderer_allocator, renderer_memory);
    memory::initialize_memory_heap(&context.resource_storage.heap, resource_memory);
    memory::initialize_memory_arena(&context.strid_storage.arena, string_id_memory);

    context.execution_commands = ALLOCATE_ARRAY(&platform_allocator, execution_command, 5);
    context.render_commands = ALLOCATE_ARRAY(&context.renderer_allocator, render_command, 1 << 12);
    context.resource_storage.resources = ALLOCATE_ARRAY(&context.renderer_allocator, rs::resource, 32);
    create_null_resource(&context.resource_storage); // Consider 0 resource being null-resource, indicating the lack of it.

    // Getting CWD

    char buffer[256] = {};
    uint32 program_path_size = win32::get_program_path(instance, buffer, ARRAY_COUNT(buffer));

    uint32 last_slash_index = 0;
    for (uint32 char_index = 0; char_index < program_path_size; char_index++) {
        if (buffer[char_index] == '\\') {
            last_slash_index = char_index;
        }
    }
    uint32 program_path_size_no_filename = last_slash_index + 1;

    // Constructing the names of game dlls

    char game_dll_buffer[256] = {};
    memory::copy(game_dll_buffer, buffer, program_path_size_no_filename);
    memory::copy(game_dll_buffer + program_path_size_no_filename, "game.dll", 8);

    char temp_dll_buffer[256] = {};
    memory::copy(temp_dll_buffer, buffer, program_path_size_no_filename);
    memory::copy(temp_dll_buffer + program_path_size_no_filename, "temp.dll", 8);

    char lock_tmp_buffer[256] = {};
    memory::copy(lock_tmp_buffer, buffer, program_path_size_no_filename);
    memory::copy(lock_tmp_buffer + program_path_size_no_filename, "lock.tmp", 8);

    // Load the game code and initialize the memory

    game_dll game = load_game_dll(game_dll_buffer, temp_dll_buffer, lock_tmp_buffer);

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

    input_devices input = {};

    int32 game_update_frequency_hz = monitor_refresh_rate_hz;
    float32 target_seconds_per_frame = 1.0f / game_update_frequency_hz;
    float32 last_frame_dt = target_seconds_per_frame;
    os::timepoint last_timepoint = win32::get_wall_clock();

    running = true;
    while (running)
    {
        reset_transitions(&input.keyboard_device);
        process_pending_messages(&input);

        uint64 dll_file_time = win32::get_file_time(game_dll_buffer);
        if (dll_file_time > game.dll.timestamp)
        {
            unload_game_dll(&game);
            game = load_game_dll(game_dll_buffer, temp_dll_buffer, lock_tmp_buffer);
        }

        if (viewport_changed)
        {
            auto viewport = gfx::make_viewport(current_client_width, current_client_height, aspect_ratio);
            gfx::set_viewport(viewport);
            viewport_changed = false;
        }

        gfx::clear();

        if (game.update_and_render)
        {
#if 0
            game.update_and_render(&context, game_memory, &input, last_frame_dt);
#else
#define FIXED_DT 0.3333f
            game.update_and_render(&context, game_memory, &input, FIXED_DT);
#undef FIXED_DT
#endif
        }

        for (usize cmd_index = 0; cmd_index < context.execution_commands.size; cmd_index++)
        {
            auto cmd = context.execution_commands[cmd_index];
            switch (cmd.type)
            {
                case execution_command::exit:
                {
                    running = false;
                }
                break;
            }
        }
        context.execution_commands.size = 0;

        for (usize cmd_index = 0; cmd_index < context.render_commands.size; cmd_index++)
        {
            auto *cmd = &context.render_commands[cmd_index];
            switch (cmd->type)
            {
                case render_command::command_type::setup_projection_matrix:
                break;

                case render_command::command_type::setup_camera:
                {
                    view = gfx::make_look_at_matrix(cmd->setup_camera.camera_position, cmd->setup_camera.look_at_position, cmd->setup_camera.camera_up_direction);
                }
                break;

                case render_command::command_type::draw_background:
                {
                    gfx::draw_background(&context, cmd);
                }
                break;

                case render_command::command_type::draw_mesh_1:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_1.mesh_token, cmd->draw_mesh_1.shader_token,
                        cmd->draw_mesh_1.model, view, projection,
                        math::vector4::zero());
                }
                break;

                case render_command::command_type::draw_mesh_with_color:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_with_color.mesh_token, cmd->draw_mesh_with_color.shader_token,
                        cmd->draw_mesh_with_color.model, view, projection,
                        cmd->draw_mesh_with_color.color);
                }
                break;
            }
        }
        context.render_commands.size = 0;

        memory::reset_allocator(&context.temporary_allocator);

#if DEBUG
        // @todo: this things should be abstract so no code from the game should appear here
        // now the names of the functions appear here.
#define DEBUG_PRINT_COUNTER(COUNTER) \
        do { \
            char buffer_[512] = {0}; \
            sprintf(buffer_, STRINGIFY(COUNTER) " took %llu cycles; %llu hits.\n",  \
                context.debug_measurements[DEBUG_TIME_SLOT_##COUNTER].cycle_count,  \
                context.debug_measurements[DEBUG_TIME_SLOT_##COUNTER].hit_count);   \
            OutputDebugStringA(buffer_); \
        } while(0);

        DEBUG_PRINT_COUNTER(update_and_render);
        DEBUG_PRINT_COUNTER(get_world_chunk_slot);
        DEBUG_PRINT_COUNTER(push_entity_in_world_chunk_slot);
        DEBUG_PRINT_COUNTER(remove_entity_from_world_chunk_slot);
        DEBUG_PRINT_COUNTER(put_entity_in_chunk);
        DEBUG_PRINT_COUNTER(move_entity_between_chunks);
        OutputDebugStringA("===================================\n");

        memory::set(context.debug_measurements, 0, sizeof(context.debug_measurements));
#endif // DEBUG

        gfx::swap_buffers(&window, &driver);

        os::timepoint end_of_frame = win32::get_wall_clock();
        last_frame_dt = get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    gfx::destroy_window_and_driver(&window, &driver);

    return 0;
}


#include <gfx/renderer.cpp>
#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <rs/resource_system.cpp>

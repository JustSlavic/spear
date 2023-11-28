// Windows
#include <platform_win32.hpp>
#include <xaudio2.h>

// Project headers
#include <base.h>
#include <game_interface.hpp>
#include <string.hpp>
#include <string_id.hpp>
#include <g3.hpp>
#include <gfx/renderer.hpp>
#include <input.hpp>
#include <audio/wav.h>

#include <stdio.h>

GLOBAL bool32 running;

GLOBAL uint32 current_client_width;
GLOBAL uint32 current_client_height;
GLOBAL bool32 viewport_changed;

#if DEBUG
GLOBAL debug_loop_state debug_loop_state = DEBUG_LOOP_IDLE;
GLOBAL memory_block debug_loop_initial_game_state;
GLOBAL array<input_state> debug_loop_inputs;
GLOBAL uint32 debug_loop_current_index;
#endif // DEBUG

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

#if DEBUG
                if (virtual_key_code == 'K')
                {
                    if (!is_down)
                    {
                        if ((debug_loop_state == DEBUG_LOOP_IDLE) && (debug_loop_inputs.size() > 0))
                        {
                            debug_loop_current_index = 0;
                            debug_loop_state = DEBUG_LOOP_REPLAYING;
                        }
                        else if (debug_loop_state == DEBUG_LOOP_REPLAYING)
                        {
                            // Nullify keyboard such as nothing is pressed on stoping the playback loop
                            // because if there's something left pressed, it will stay pressed although nothing is
                            // pressed on the actual keyboard
                            memory__set(input, 0, sizeof(input_state));
                            debug_loop_state = DEBUG_LOOP_IDLE;
                        }
                    }
                }

                if (virtual_key_code == 'L')
                {
                    if (!is_down)
                    {
                        if (debug_loop_state == DEBUG_LOOP_IDLE) {
                            debug_loop_inputs.clear();
                            debug_loop_state = DEBUG_LOOP_RECORDING;
                        } else if (debug_loop_state == DEBUG_LOOP_RECORDING) {
                            debug_loop_current_index = 0;
                            debug_loop_state = DEBUG_LOOP_REPLAYING;
                        } else if (debug_loop_state == DEBUG_LOOP_REPLAYING) {
                            // Nullify keyboard controller such as nothing is pressed on stoping the playback loop
                            // because if there's something left pressed, it will stay pressed although nothing is
                            // pressed on the actual keyboard
                            *input = {};
                            debug_loop_state = DEBUG_LOOP_IDLE;
                        }
                    }
                }
#endif // DEBUG
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
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    auto chosen_api = gfx__api::opengl;
    win32::window window = {};
    gfx__driver driver = {};

    if (chosen_api == gfx__api::opengl)
    {
        win32::create_opengl_window(instance, 1600, 900, window_callback, &window);
        gfx__initialize_opengl(&window, &driver);
    }
    else if (chosen_api == gfx__api::dx11)
    {
        win32::create_simple_window(instance, 800, 600, window_callback, &window);
        // gfx__initialize_dx11(&window, &driver);
    }

    gfx__set_clear_color(0, 0, 0, 1);
    int32 monitor_refresh_rate_hz = GetDeviceCaps(((win32::window *) &window)->device_context, VREFRESH);
    gfx__vsync(&window, true);
    gfx__depth_test(&driver, true);
    gfx__write_depth(&driver, true);

    memory_block global_memory = win32::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));

    execution_context context;
    memory__set(&context, 0, sizeof(execution_context));

    memory_allocator global_allocator = memory_allocator__create_arena_from_memory_block(global_memory);
    memory_allocator platform_allocator = memory_allocator__create_arena(global_allocator, MEGABYTES(20));
    context.temporary_allocator = memory_allocator__create_arena(global_allocator, MEGABYTES(10));
    context.renderer_allocator = memory_allocator__create_arena(global_allocator, MEGABYTES(2));
    context.rs.heap = memory_allocator__create_arena(global_allocator, MEGABYTES(1));
    context.strid_storage = initialize_string_id_storage(ALLOCATE_BUFFER(global_allocator, MEGABYTES(1)));

    context.execution_commands = ALLOCATE_ARRAY(platform_allocator, execution_command, 5);
    context.render_commands = ALLOCATE_ARRAY(context.renderer_allocator, render_command, 1 << 12);

    context.debug_load_file = win32::load_file;

#if DEBUG
    memory_block game_memory = ALLOCATE_BUFFER(global_allocator, MEGABYTES(5));
    debug_loop_initial_game_state = ALLOCATE_BUFFER_(platform_allocator, game_memory.size);
    debug_loop_inputs = ALLOCATE_ARRAY_(platform_allocator, input_state, 60*100);
#endif // DEBUG

    resource_token screen_frame_mesh = {};
    resource_token screen_frame_shader = {};
    {
        screen_frame_shader = create_shader_resource(&context.rs, make_string_id(context.strid_storage, "frame.shader"));

        // 3--------2
        // |\      /|
        // | 7----6 |
        // | |    | |
        // | 4----5 |
        // |/      \|
        // 0--------1

        struct frame_vertex
        {
            float32 x, y;
            float32 w, h;
        };

        frame_vertex vbo_[] = {
            frame_vertex{ -1.f, -1.f,  0.f,  0.f },
            frame_vertex{  1.f, -1.f,  0.f,  0.f },
            frame_vertex{  1.f,  1.f,  0.f,  0.f },
            frame_vertex{ -1.f,  1.f,  0.f,  0.f },
            frame_vertex{ -1.f, -1.f,  1.f,  1.f },
            frame_vertex{  1.f, -1.f, -1.f,  1.f },
            frame_vertex{  1.f,  1.f, -1.f, -1.f },
            frame_vertex{ -1.f,  1.f,  1.f, -1.f },
        };

        auto vbo = ALLOCATE_BUFFER_(context.temporary_allocator, sizeof(vbo_));
        memory__copy(vbo.memory, vbo_, sizeof(vbo_));

        uint32 ibo_[] = {
            0, 4, 3,
            3, 4, 7,
            0, 1, 4,
            4, 1, 5,
            5, 1, 2,
            5, 2, 6,
            7, 6, 2,
            7, 2, 3,
        };

        auto ibo = ALLOCATE_BUFFER_(context.temporary_allocator, sizeof(ibo_));
        memory__copy(ibo.memory, ibo_, sizeof(ibo_));

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 2);
        gfx::push_layout_element(&vbl, 2);
        screen_frame_mesh = create_mesh_resource(&context.rs, vbo, ibo, vbl);
    }

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
    memory__copy(game_dll_buffer, buffer, program_path_size_no_filename);
    memory__copy(game_dll_buffer + program_path_size_no_filename, "game.dll", 8);

    char temp_dll_buffer[256] = {};
    memory__copy(temp_dll_buffer, buffer, program_path_size_no_filename);
    memory__copy(temp_dll_buffer + program_path_size_no_filename, "temp.dll", 8);

    char lock_tmp_buffer[256] = {};
    memory__copy(lock_tmp_buffer, buffer, program_path_size_no_filename);
    memory__copy(lock_tmp_buffer + program_path_size_no_filename, "lock.tmp", 8);

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

    auto view = matrix4__identity();
    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gfx__make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    input_state input = {};

    int32 game_update_frequency_hz = monitor_refresh_rate_hz;
    float32 target_seconds_per_frame = 1.0f / game_update_frequency_hz;
    float32 last_frame_dt = target_seconds_per_frame;
    timepoint last_timepoint = win32::get_wall_clock();

// --------------------------------------------------------------------------------
#if 0
    IXAudio2 *AudioDevice = NULL;
    HRESULT Result = XAudio2Create(&AudioDevice, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(Result))
    {
        // ErrorBeepBoop("Cannot initialize XAudio2 Device!");
        return 1;
    }

    uint32 audio_channel_count = 2;
    uint32 audio_samples_per_second = 48000;

    IXAudio2MasteringVoice *MasterVoice = NULL;
    Result = AudioDevice->CreateMasteringVoice(&MasterVoice, audio_channel_count, audio_samples_per_second);

    auto wav_contents = win32::load_file(platform_allocator, "birds.wav");
    audio__track bg_music = load_wav(wav_contents);

    WAVEFORMATEX wave_format;
    wave_format.wFormatTag = WAVE_FORMAT_PCM;
    wave_format.nChannels = (WORD) bg_music.channel_count;
    wave_format.nSamplesPerSec = bg_music.samples_per_second;
    wave_format.wBitsPerSample = (WORD) bg_music.bits_per_sample;
    wave_format.nBlockAlign = (wave_format.nChannels * wave_format.wBitsPerSample) / 8;
    wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;
    wave_format.cbSize = 0;

    IXAudio2SourceVoice* SourceVoice = NULL;
    Result = AudioDevice->CreateSourceVoice(&SourceVoice, (WAVEFORMATEX *) &wave_format);
    if (FAILED(Result))
    {
        // ErrorBeepBoop("Cannot create Source Voice!");
        return 1;
    }

    XAUDIO2_BUFFER audio_buffer;
    ZeroMemory(&audio_buffer, sizeof(audio_buffer));
    audio_buffer.Flags = XAUDIO2_END_OF_STREAM;
    audio_buffer.AudioBytes = (uint32) bg_music.size;
    audio_buffer.pAudioData = (BYTE const *) bg_music.data;
    audio_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    Result = SourceVoice->SubmitSourceBuffer(&audio_buffer);
    if (FAILED(Result))
    {
        // ErrorBeepBoop("Cannot submit audio_buffer for some reason");
        return 1;
    }

    SourceVoice->SetVolume(0.1f, XAUDIO2_COMMIT_NOW);
    Result = SourceVoice->Start(0);
    if (FAILED(Result))
    {
        // ErrorBeepBoop("Cannot play a voice for some reason");
        return 1;
    }
#endif
// --------------------------------------------------------------------------------

    running = true;
    while (running)
    {
        reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
        reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
        process_pending_messages(&input);
        win32::get_mouse_pos(&window, &input.mouse.x, &input.mouse.y);
        input.dt = last_frame_dt;
        input.time = (float32) win32::get_seconds(last_timepoint);

        uint64 dll_file_time = win32::get_file_time(game_dll_buffer);
        if (dll_file_time > game.dll.timestamp)
        {
            unload_game_dll(&game);
            game = load_game_dll(game_dll_buffer, temp_dll_buffer, lock_tmp_buffer);
        }

        if (viewport_changed)
        {
            auto viewport = gfx__make_viewport(current_client_width, current_client_height, aspect_ratio);
            gfx__set_viewport(viewport);
            viewport_changed = false;

            // @todo
            context.screen_width = 0;
            context.screen_height = 0;

            context.window_width = current_client_width;
            context.window_height = current_client_height;

            context.letterbox_width = viewport.width;
            context.letterbox_height = viewport.height;
        }

        gfx__clear();

#if DEBUG
        if (debug_loop_state == DEBUG_LOOP_RECORDING)
        {
            if (debug_loop_inputs.empty())
            {
                // Just started recording, save initial game memory
                memory__copy(debug_loop_initial_game_state.memory, game_memory.memory, game_memory.size);
                debug_loop_current_index = 0;
            }

            if (debug_loop_inputs.size() < debug_loop_inputs.capacity())
            {
                debug_loop_inputs.push(input);
            }
            else
            {
                ASSERT_FAIL("The space for storing input is ran out. Increase it in code above.");
            }
        }

        if (debug_loop_state == DEBUG_LOOP_REPLAYING)
        {
            if (debug_loop_current_index == 0)
            {
                // The playback loop started over again, copy saved initial game memory back to use.
                memory__copy(game_memory.memory, debug_loop_initial_game_state.memory, game_memory.size);
            }

            input = debug_loop_inputs[debug_loop_current_index];

            debug_loop_current_index = (debug_loop_current_index + 1) % debug_loop_inputs.size();
        }
#endif // DEBUG

        if (game.update_and_render)
        {
            game.update_and_render(&context, game_memory, &input);
        }

        for (usize cmd_index = 0; cmd_index < context.execution_commands.size(); cmd_index++)
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
        context.execution_commands.clear();

        for (usize cmd_index = 0; cmd_index < context.render_commands.size(); cmd_index++)
        {
            auto *cmd = &context.render_commands[cmd_index];
            switch (cmd->type)
            {
                case render_command::command_type::setup_projection_matrix:
                break;

                case render_command::command_type::setup_camera:
                {
                    view = gfx__make_look_at_matrix(cmd->setup_camera.camera_position, cmd->setup_camera.look_at_position, cmd->setup_camera.camera_up_direction);
                }
                break;

                case render_command::command_type::draw_background:
                {
                    gfx__write_depth(&driver, false);
                    gfx__draw_background(&context, cmd);
                    gfx__write_depth(&driver, true);
                }
                break;

                case render_command::command_type::draw_mesh_with_color:
                {
                    gfx__draw_polygon_simple(&context,
                        cmd->draw_mesh_with_color.mesh_token, cmd->draw_mesh_with_color.shader_token,
                        cmd->draw_mesh_with_color.model, view, projection,
                        cmd->draw_mesh_with_color.color);
                }
                break;

                case render_command::command_type::draw_mesh_with_texture:
                {
                    gfx__draw_rectangle_texture(&context,
                        cmd->draw_mesh_with_texture.mesh_token,
                        cmd->draw_mesh_with_texture.shader_token,
                        cmd->draw_mesh_with_texture.texture_token,
                        cmd->draw_mesh_with_texture.model, view, projection);
                }
                break;
                case render_command::command_type::draw_screen_frame:
                {
                    gfx__draw_polygon_simple(&context,
                        screen_frame_mesh,
                        screen_frame_shader,
                        cmd->draw_screen_frame.model,
                        cmd->draw_screen_frame.view,
                        cmd->draw_screen_frame.projection,
                        cmd->draw_screen_frame.color);
                }
                break;

                case render_command::command_type::draw_ui:
                {
                    gfx__draw_polygon_simple(&context,
                        cmd->draw_ui.mesh_token,
                        cmd->draw_ui.shader_token,
                        cmd->draw_ui.model,
                        cmd->draw_ui.view,
                        cmd->draw_ui.projection,
                        cmd->draw_ui.color);
                }
                break;

                case render_command::command_type::draw_ui_texture:
                {
                    gfx__draw_rectangle_texture(&context,
                        cmd->draw_ui_texture.mesh_token,
                        cmd->draw_ui_texture.shader_token,
                        cmd->draw_ui_texture.texture_token,
                        cmd->draw_ui_texture.model,
                        cmd->draw_ui_texture.view,
                        cmd->draw_ui_texture.projection);
                }
                break;
            }
        }
        context.render_commands.clear();

        memory_allocator__reset(context.temporary_allocator);

#if DEBUG & 0
        // @todo: this things should be abstract so no code from the game should appear here
        // now the names of the functions appear here.
#define DEBUG_PRINT_COUNTER(COUNTER) \
        do { \
            char buffer_[512] = {0}; \
            sprintf(buffer_, STRINGIFY(COUNTER) " took %llu cycles; %llu hits; (%f cycles per hit)\n",  \
                context.debug_measurements[DEBUG_TIME_SLOT_##COUNTER].cycle_count, \
                context.debug_measurements[DEBUG_TIME_SLOT_##COUNTER].hit_count, \
                (float32) context.debug_measurements[DEBUG_TIME_SLOT_##COUNTER].cycle_count / (float32) context.debug_measurements[DEBUG_TIME_SLOT_##COUNTER].hit_count \
                ); \
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

        auto debug_loop_frame_color =
            (debug_loop_state == DEBUG_LOOP_RECORDING) ? V4(1, 1, 0, 1) :
            (debug_loop_state == DEBUG_LOOP_REPLAYING) ? V4(0, 1, 0, 1) :
            V4(1, 0, 0, 1);

        // if (debug_loop_state == DEBUG_LOOP_RECORDING || debug_loop_state == DEBUG_LOOP_REPLAYING)
        {
            auto debug_loop_model = math::matrix4__identity();
            auto debug_loop_view  = math::matrix4__identity();
            auto debug_loop_projection = math::matrix4__identity();
            gfx::draw_polygon_simple(&context,
                screen_frame_mesh, screen_frame_shader,
                debug_loop_model, debug_loop_view, debug_loop_projection,
                debug_loop_frame_color);
        }
#endif // DEBUG

        gfx__swap_buffers(&window, &driver);

        timepoint end_of_frame = win32::get_wall_clock();
        last_frame_dt = (float32) win32::get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    gfx__destroy_window_and_driver(&window, &driver);

    return 0;
}


#include <gfx/renderer.cpp>
#include <memory_allocator.c>
#include <string_id.cpp>
#include <rs/resource_system.cpp>
#include <image/bmp.cpp>
#include <audio/wav.c>

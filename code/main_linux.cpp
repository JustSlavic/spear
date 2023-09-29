// Linux
#include <platform_linux.hpp>

// Project headers
#include <base.h>
#include <game_interface.hpp>
#include <gfx/renderer.hpp>
#include <rs/resource.hpp>
#include <input.hpp>


#define MOUSE_LMB                    1
#define MOUSE_MMB                    2
#define MOUSE_RMB                    3
#define MOUSE_WHEEL_UP               4
#define MOUSE_WHEEL_DOWN             5
#define MOUSE_MB_1                   8
#define MOUSE_MB_2                   9

#define GAMEPAD_EVENT_BUTTON         0x01    /* button pressed/released */
#define GAMEPAD_EVENT_AXIS           0x02    /* joystick moved */
#define GAMEPAD_EVENT_INIT           0x80    /* initial state of device */

#define GAMEPAD_AXIS_MIN             (-32767)
#define GAMEPAD_AXIS_MAX             (32767)

#define GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define GAMEPAD_TRIGGER_DEADZONE     7710


GLOBAL bool32 running = true;
GLOBAL uint32 current_display_width;
GLOBAL uint32 current_display_height;
GLOBAL uint32 display_size_changed;


void process_pending_messages(linux::window *window, input_state *input)
{
    XEvent event;
    while (XPending(window->x_display))
    {
        XNextEvent(window->x_display, &event);

        //
        // @note: X11 key-repeat BS:
        // The X-Server always sends you repeated keys. It's bad, because I want
        // to be precise when player hits/releases the keyboard key.
        // The event stream looks like this:
        //
        //    ... [real-press] ... [repeat-release]-[repeat-press]  [repeat-release]-[repeat-press]  [real-release] ...
        //
        // Repeat events always go in pairs: RELEASE-PRESS, and always have the same timestamp.
        // I detect that in the code below, and just skip them.
        //
        XEvent next_event;
        if (XPending(window->x_display))
        {
            // @note: XPeekEvent is blocking call. XPending makes sure I call XPeekEvent only when
            // there are events present in the queue.
            XPeekEvent(window->x_display, &next_event);
            if ((event.type == KeyRelease) && (next_event.type == KeyPress))
            {
                if (event.xkey.time == next_event.xkey.time)
                {
                    XNextEvent(window->x_display, &next_event); // Poll next event from the queue
                    continue; // Skip
                }
            }
        }

        switch (event.type)
        {
            case MotionNotify:
            {
                input->mouse.x = event.xmotion.x;
                input->mouse.y = event.xmotion.y;
            }
            break;

            case ButtonPress:
            case ButtonRelease:
            {
                bool32 is_down = (event.type == ButtonPress);
                switch (event.xbutton.button)
                {
                    case MOUSE_LMB: process_button_state(&input->mouse[MOUSE_LEFT], is_down);
                        break;
                    case MOUSE_MMB: process_button_state(&input->mouse[MOUSE_MIDDLE], is_down);
                        break;
                    case MOUSE_RMB: process_button_state(&input->mouse[MOUSE_RIGHT], is_down);
                        break;
                }
            }
            break;

            case KeyPress:
            case KeyRelease:
            {
                bool32 is_down = (event.type == KeyPress);
                process_button_state(&input->keyboard[linux::map_button_from_virtual_key_code(event.xkey.keycode)], is_down);
            }
            break;

            // printf("keycode release: %d\n", e.keycode);
            // print_binary(e.state);
            // printf("\n");
            // printf(
            //     "[ B1 B2 B3 B4 B5 Shift Lock Control M1 M2 M3 M4 M5 ]\n"
            //     "[ %2d %2d %2d %2d %2d %5d %4d %7d %2d %2d %2d %2d %2d ]\n",
            //     (e.state & Button1Mask) > 0, // LMB
            //     (e.state & Button2Mask) > 0, // MMB
            //     (e.state & Button3Mask) > 0, // RMB
            //     (e.state & Button4Mask) > 0, //
            //     (e.state & Button5Mask) > 0, //
            //     (e.state & ShiftMask) > 0,   // Shift
            //     (e.state & LockMask) > 0,    // CapsLock
            //     (e.state & ControlMask) > 0, //
            //     (e.state & Mod1Mask) > 0,    //
            //     (e.state & Mod2Mask) > 0,    // NumLock
            //     (e.state & Mod3Mask) > 0,    //
            //     (e.state & Mod4Mask) > 0,    // window/Super Key
            //     (e.state & Mod5Mask) > 0);   //

            case ClientMessage:
                running = false;
            break;

            case ConfigureNotify:
            {
                // printf("x=%d; y=%d\n", event.xconfigure.x, event.xconfigure.y);
                current_display_width = event.xconfigure.width;
                current_display_height = event.xconfigure.height;
                display_size_changed = true;
            }
            break;
        }
    }
}


int main(int argc, char **argv, char **env)
{
    linux::window window = {};
    gfx::driver driver = {};

    linux::create_opengl_window(1600, 900, &window, &driver);
    gfx::initialize_opengl(&driver);

    auto monitor = linux::get_monitor_stats(&window);

    gfx::set_clear_color(0, 0, 0, 1);
    int32 monitor_refresh_rate_hz = monitor.refresh_rate;
    gfx::vsync(&window, true);

    execution_context context;
    memory__set(&context, 0, sizeof(execution_context));
    memory_block global_memory = linux::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));

    memory_allocator global_allocator = memory_allocator__create_arena_from_memory_block(global_memory);
    memory_allocator platform_allocator = memory_allocator__create_arena(global_allocator, MEGABYTES(20));
    context.temporary_allocator = memory_allocator__create_arena(global_allocator, MEGABYTES(10));
    context.renderer_allocator = memory_allocator__create_arena(global_allocator, MEGABYTES(2));
    context.resource_storage.heap = memory_allocator__create_arena(global_allocator, MEGABYTES(1));
    context.strid_storage = initialize_string_id_storage(ALLOCATE_BUFFER(global_allocator, MEGABYTES(1)));

    context.execution_commands = ALLOCATE_ARRAY(platform_allocator, execution_command, 5);
    context.render_commands = ALLOCATE_ARRAY(context.renderer_allocator, render_command, 1 << 12);
    context.resource_storage.resources = ALLOCATE_ARRAY(context.renderer_allocator, rs::resource, 32);
    rs::create_null_resource(&context.resource_storage); // Consider 0 resource being null-resource, indicating the lack of it.

    context.debug_load_file = linux::load_file;

    memory_block game_memory = ALLOCATE_BUFFER(global_allocator, MEGABYTES(5));
    initialize_memory(&context, game_memory);

    rs::resource_token screen_frame_mesh = {};
    rs::resource_token screen_frame_shader = {};
        {
        screen_frame_shader = create_shader_resource(&context.resource_storage, make_string_id(context.strid_storage, "frame.shader"));

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
        screen_frame_mesh = create_mesh_resource(&context.resource_storage, vbo, ibo, vbl);
    }

    auto view = matrix4__identity();
    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gfx::make_projection_matrix_fov(math::to_radians(static_cast<float32>(60)), aspect_ratio, 0.05f, 100.0f);

    input_state input = {};

    auto time_resolution = linux::get_wall_clock_frequency();
    UNUSED(time_resolution);

    int32 game_update_frequency_hz = monitor_refresh_rate_hz;
    float32 target_seconds_per_frame = 1.0f / game_update_frequency_hz;
    float32 last_frame_dt = target_seconds_per_frame;
    timepoint last_timepoint = linux::get_wall_clock();

    running = true;
    while (running)
    {
        reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
        reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
        process_pending_messages(&window, &input);
        input.dt = last_frame_dt;

        if (display_size_changed)
        {
            auto viewport = gfx::make_viewport(current_display_width, current_display_height, aspect_ratio);
            gfx::set_viewport(viewport);
            display_size_changed = false;

            // @todo: get screen (monitor) width and height in pixels
            context.screen_width = 0;
            context.screen_height = 0;

            context.window_width = current_display_width;
            context.window_height = current_display_height;

            context.letterbox_width = viewport.width;
            context.letterbox_height = viewport.height;
        }

        gfx::clear();

        update_and_render(&context, game_memory, &input);

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

                default:
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
                    view = gfx::make_look_at_matrix(cmd->setup_camera.camera_position, cmd->setup_camera.look_at_position, cmd->setup_camera.camera_up_direction);
                }
                break;

                case render_command::command_type::draw_background:
                {
                    gfx::draw_background(&context, cmd);
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

                case render_command::command_type::draw_mesh_with_texture:
                {
                    gfx::draw_rectangle_texture(&context,
                        cmd->draw_mesh_with_texture.mesh_token,
                        cmd->draw_mesh_with_texture.shader_token,
                        cmd->draw_mesh_with_texture.texture_token,
                        cmd->draw_mesh_with_texture.model, view, projection);
                }
                break;
                case render_command::command_type::draw_screen_frame:
                {
                    gfx::draw_polygon_simple(&context,
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
                    gfx::draw_polygon_simple(&context,
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
                    gfx::draw_rectangle_texture(&context,
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

        gfx::swap_buffers(&window, &driver);

        timepoint end_of_frame = linux::get_wall_clock();
        last_frame_dt = (float32) linux::get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    gfx::destroy_window_and_driver(&window, &driver);
    return 0;
}

#include <gfx/renderer.cpp>
#include <memory_allocator.h>
#include <string_id.cpp>
#include <game_platformer/game.cpp>
#include <rs/resource_system.cpp>
#include <image/bmp.cpp>
#include <memory_allocator.c>
#include <image/png.cpp>
#include <memory/crc.cpp>

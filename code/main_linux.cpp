// Linux
#include <platform_linux.hpp>

// Project headers
#include <base.hpp>
#include <game_interface.hpp>
#include <gfx/renderer.hpp>
#include <rs/resource.hpp>
#include <input.hpp>


#define KEYCODE_ESC                  9
#define KEYCODE_Q                    24
#define KEYCODE_W                    25
#define KEYCODE_E                    26
#define KEYCODE_R                    27
#define KEYCODE_T                    28
#define KEYCODE_Y                    29
#define KEYCODE_U                    30
#define KEYCODE_I                    31
#define KEYCODE_O                    32
#define KEYCODE_P                    33
#define KEYCODE_BRACKET_OPEN         34
#define KEYCODE_BRACKET_CLOSE        35
#define KEYCODE_A                    38
#define KEYCODE_S                    39
#define KEYCODE_D                    40
#define KEYCODE_Z                    52
#define KEYCODE_SPACE                65
#define KEYCODE_F1                   67
#define KEYCODE_F2                   68
#define KEYCODE_F3                   69
#define KEYCODE_F4                   70
#define KEYCODE_F5                   71
#define KEYCODE_F6                   72
#define KEYCODE_F7                   73
#define KEYCODE_F8                   74
#define KEYCODE_F9                   75
#define KEYCODE_F10                  76
#define KEYCODE_F11                  95
#define KEYCODE_F12                  96
#define KEYCODE_UP                   111
#define KEYCODE_LEFT                 113
#define KEYCODE_RIGHT                114
#define KEYCODE_DOWN                 116
#define KEYCODE_CTRL                 37
#define KEYCODE_SHIFT                50

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


void process_pending_messages(linux::window *window, input *input)
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
            case ButtonPress:
            case ButtonRelease:
            {
                // b32 is_down = (event.type == ButtonPress);

                // if (event.xbutton.button == MOUSE_LMB)
                // {
                //     linux_process_key_event(&mouse->LMB, is_down);
                // }
                // else if (event.xbutton.button == MOUSE_MMB)
                // {
                //     linux_process_key_event(&mouse->MMB, is_down);
                // }
                // else if (event.xbutton.button == MOUSE_RMB)
                // {
                //     linux_process_key_event(&mouse->RMB, is_down);
                // }
            }
            break;

            case KeyPress:
            case KeyRelease:
            {
                bool32 is_down = (event.type == KeyPress);
                switch (event.xkey.keycode)
                {
                    case KEYCODE_ESC: process_button_state(&input->keyboard[keyboard_device::ESC], is_down);
                        break;
                    case KEYCODE_W: process_button_state(&input->keyboard[keyboard_device::W], is_down);
                        break;
                    case KEYCODE_A: process_button_state(&input->keyboard[keyboard_device::A], is_down);
                        break;
                    case KEYCODE_S: process_button_state(&input->keyboard[keyboard_device::S], is_down);
                        break;
                    case KEYCODE_D: process_button_state(&input->keyboard[keyboard_device::D], is_down);
                        break;
                    case KEYCODE_Y: process_button_state(&input->keyboard[keyboard_device::Y], is_down);
                        break;
                }
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
    int32 display_width = 2400;
    int32 display_height = 1600;

    linux::window window = {};
    gfx::driver driver = {};

    linux::create_opengl_window(display_width, display_height, &window, &driver);
    gfx::initialize_opengl(&driver);

    auto monitor = get_monitor_stats(&window);

    gfx::set_clear_color(0, 0, 0, 1);
    int32 monitor_refresh_rate_hz = monitor.refresh_rate;
    gfx::vsync(&window, true);

    memory_block global_memory = linux::allocate_memory((void *) TERABYTES(1), MEGABYTES(30));

    memory::allocator global_allocator;
    memory::initialize_memory_arena(&global_allocator, global_memory);

    memory_block platform_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block game_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(20));
    memory_block scratchpad_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block renderer_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(4));
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

    initialize_memory(&context, game_memory);

    auto view = math::matrix4::identity();
    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gfx::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    input input = {};

    auto time_resolution = linux::get_wall_clock_frequency();
    UNUSED(time_resolution);

    int32 game_update_frequency_hz = monitor_refresh_rate_hz;
    float32 target_seconds_per_frame = 1.0f / game_update_frequency_hz;
    float32 last_frame_dt = target_seconds_per_frame;
    timepoint last_timepoint = linux::get_wall_clock();

    running = true;
    while (running)
    {
        reset_transitions(input.keyboard.buttons, keyboard_device::KEY_COUNT);
        process_pending_messages(&window, &input);
        input.dt = last_frame_dt;

        if (display_size_changed)
        {
            auto viewport = gfx::make_viewport(current_display_width, current_display_height, aspect_ratio);
            gfx::set_viewport(viewport);
            display_size_changed = false;

            context.screen_width = 0;
            context.screen_height = 0;

            context.window_width = current_display_width;
            context.window_height = current_display_height;

            context.letterbox_width = viewport.width;
            context.letterbox_height = viewport.height;
        }

        gfx::clear();

        update_and_render(&context, game_memory, &input);

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

            default:
                break;
            }
        }

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

                case render_command::command_type::draw_mesh_with_color:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_with_color.mesh_token, cmd->draw_mesh_with_color.shader_token,
                        cmd->draw_mesh_with_color.model, view, projection,
                        cmd->draw_mesh_with_color.color);
                }
                break;

                case render_command::command_type::draw_screen_frame:
                break;

                case render_command::command_type::draw_ui:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_ui.mesh_token, cmd->draw_ui.shader_token,
                        cmd->draw_ui.model, cmd->draw_ui.view, cmd->draw_ui.projection,
                        cmd->draw_ui.color);
                }
                break;
            }
        }
        context.render_commands.size = 0;
        memory::reset_allocator(&context.temporary_allocator);

        gfx::swap_buffers(&window, &driver);

        timepoint end_of_frame = linux::get_wall_clock();
        last_frame_dt = (float32) linux::get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    gfx::destroy_window_and_driver(&window, &driver);
    return 0;
}

#include <gfx/renderer.cpp>
#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <game_platformer/game.cpp>

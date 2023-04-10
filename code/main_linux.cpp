// Linux
#include <platform_linux.hpp>

// Project headers
#include <base.hpp>
#include <game_interface.hpp>
#include <gfx/renderer.hpp>
#include <input.hpp>



#define KEYCODE_ESC                  9
#define KEYCODE_W                    25
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


void process_pending_messages(linux::window *window, input_devices *input)
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
                    case KEYCODE_ESC: process_button_state(&input->keyboard_device[keyboard::esc], is_down);
                        break;
                    case KEYCODE_W: process_button_state(&input->keyboard_device[keyboard::w], is_down);
                        break;
                    case KEYCODE_A: process_button_state(&input->keyboard_device[keyboard::a], is_down);
                        break;
                    case KEYCODE_S: process_button_state(&input->keyboard_device[keyboard::s], is_down);
                        break;
                    case KEYCODE_D: process_button_state(&input->keyboard_device[keyboard::d], is_down);
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

    memory_block global_memory = linux::allocate_memory((void *) TERABYTES(1), MEGABYTES(30));

    memory::allocator global_allocator;
    memory::initialize_memory_arena(&global_allocator, global_memory);

    memory_block game_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(20));
    memory_block scratchpad_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block renderer_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(4));
    memory_block resource_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block string_id_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));

    execution_context context = {};
    context.resource_storage.resource_count = 1; // Consider 0 resource being null-resource, indicating the lack of it.

    memory::initialize_memory_arena(&context.temporary_allocator, scratchpad_memory);
    memory::initialize_memory_arena(&context.renderer_allocator, renderer_memory);
    memory::initialize_memory_heap(&context.resource_storage.heap, resource_memory);
    memory::initialize_memory_arena(&context.strid_storage.arena, string_id_memory);

    context.render_command_queue = (gfx::render_command *) ALLOCATE_BUFFER_(&context.renderer_allocator, sizeof(gfx::render_command)*3000);
    context.render_command_queue_capacity = 3000;

    initialize_memory(&context, game_memory);
    gfx::set_clear_color(0, 0, 0, 1);

    auto view = math::matrix4::identity();
    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gfx::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    input_devices input = {};

    while (running)
    {
        reset_transitions(&input.keyboard_device);
        process_pending_messages(&window, &input);

        if (display_size_changed)
        {
            auto viewport = gfx::make_viewport(current_display_width, current_display_height, aspect_ratio);
            gfx::set_viewport(viewport);
            display_size_changed = false;
        }

        gfx::clear();

        update_and_render(&context, game_memory, &input, 0.333);

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

            default:
                break;
            }
        }

        for (usize cmd_index = 0; cmd_index < context.render_command_queue_size; cmd_index++)
        {
            auto *cmd = context.render_command_queue + cmd_index;
            switch (cmd->type)
            {
                case gfx::render_command::command_type::setup_projection_matrix:
                break;

                case gfx::render_command::command_type::setup_camera:
                {
                    view = gfx::make_look_at_matrix(cmd->setup_camera.camera_position, cmd->setup_camera.look_at_position, cmd->setup_camera.camera_up_direction);
                }
                break;

                case gfx::render_command::command_type::draw_background:
                {
                    gfx::draw_background(&context, cmd);
                }
                break;

                case gfx::render_command::command_type::draw_mesh_1:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_1.mesh_token, cmd->draw_mesh_1.shader_token,
                        cmd->draw_mesh_1.model, view, projection,
                        math::vector4::zero());
                }
                break;

                case gfx::render_command::command_type::draw_mesh_with_color:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_with_color.mesh_token, cmd->draw_mesh_with_color.shader_token,
                        cmd->draw_mesh_with_color.model, view, projection,
                        cmd->draw_mesh_with_color.color);
                }
                break;
            }
        }
        context.render_command_queue_size = 0;
        memory::reset_allocator(&context.temporary_allocator);

        gfx::swap_buffers(&window, &driver);
    }

    gfx::destroy_window_and_driver(&window, &driver);
    return 0;
}

#include <gfx/renderer.cpp>
#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <game.cpp>

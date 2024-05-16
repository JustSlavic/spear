#include <SDL2/SDL.h>

#include <base.h>
#include <time.hpp>
#include <input.hpp>
// #include <gfx/renderer.hpp>
#include <game_interface.hpp>
#include <platform_sdl.hpp>


GLOBAL bool32 running;
GLOBAL uint32 current_client_width;
GLOBAL uint32 current_client_height;


void process_pending_messages(input_state *input)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type)
        {
            case SDL_QUIT:
                running = false;
            break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE: process_button_state(&input->keyboard[KB_ESC], event.type == SDL_KEYDOWN);
                        break;

                    default:
                        break;
                }
            }
            break;
        }
    }
}


int main()
{
    // current_client_width = 800;
    // current_client_height = 600;

    // int32 err = SDL_Init(SDL_INIT_VIDEO);
    // UNUSED(err);
    // SDL_Window *window = SDL_CreateWindow(
    //             "window",
    //             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //             current_client_width, current_client_height,
    //             SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    // SDL_Surface* surface = SDL_GetWindowSurface(window);
    // UNUSED(surface);
    // SDL_GLContext opengl_context = SDL_GL_CreateContext(window);
    // gfx::driver driver = {};
    // gfx::initialize_opengl(&driver);

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // SDL_version compiled;
    // SDL_VERSION(&compiled);
    // UNUSED(compiled);

    // SDL_version linked;
    // SDL_GetVersion(&linked);
    // UNUSED(linked);

    // memory_block global_memory = sdl::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));

    // memory::allocator global_allocator;
    // memory::initialize_memory_arena(&global_allocator, global_memory);

    // memory_block platform_memory   = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(20));
    // memory_block game_memory       = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(5));
    // memory_block scratchpad_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    // memory_block renderer_memory   = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(2));
    // memory_block resource_memory   = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    // memory_block string_id_memory  = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));

    // memory::allocator platform_allocator = {};
    // memory::initialize_memory_arena(&platform_allocator, platform_memory);

    // execution_context context = {};
    // memory::initialize_memory_arena(&context.temporary_allocator, scratchpad_memory);
    // memory::initialize_memory_arena(&context.renderer_allocator, renderer_memory);
    // memory::initialize_memory_heap(&context.resource_storage.heap, resource_memory);

    // context.strid_storage = initialize_string_id_storage(string_id_memory);

    // context.execution_commands = ALLOCATE_ARRAY(&platform_allocator, execution_command, 5);
    // context.render_commands = ALLOCATE_ARRAY(&context.renderer_allocator, render_command, 1 << 12);
    // context.resource_storage.resources = ALLOCATE_ARRAY(&context.renderer_allocator, rs::resource, 32);
    // create_null_resource(&context.resource_storage); // Consider 0 resource being null-resource, indicating the lack of it.

    // initialize_memory(&context, game_memory);

    // auto view = math::matrix4::identity();
    // float32 aspect_ratio = 16.0f / 9.0f;
    // auto projection = gfx::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    // input_state input = {};

    // float32 last_frame_dt = 60.f;
    // timepoint last_timepoint;
    // last_timepoint.counts = SDL_GetTicks64();

    // running = true;
    // while (running)
    // {
    //     reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
    //     reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
    //     process_pending_messages(&input);
    //     // win32::get_mouse_pos(&window, &input.mouse.x, &input.mouse.y);
    //     input.dt = last_frame_dt;
    //     input.time = ((float32) last_timepoint.counts) / 1000000.f;

    //     update_and_render(&context, game_memory, &input);

    //     for (usize cmd_index = 0; cmd_index < context.execution_commands.size(); cmd_index++)
    //     {
    //         auto cmd = context.execution_commands[cmd_index];
    //         switch (cmd.type)
    //         {
    //             case execution_command::exit:
    //             {
    //                 running = false;
    //             }
    //             break;
    //         }
    //     }
    //     context.execution_commands.clear();

    //     for (usize cmd_index = 0; cmd_index < context.render_commands.size(); cmd_index++)
    //     {
    //         auto *cmd = &context.render_commands[cmd_index];
    //         switch (cmd->type)
    //         {
    //             case render_command::command_type::setup_projection_matrix:
    //             break;

    //             case render_command::command_type::setup_camera:
    //             {
    //                 view = gfx::make_look_at_matrix(cmd->setup_camera.camera_position, cmd->setup_camera.look_at_position, cmd->setup_camera.camera_up_direction);
    //             }
    //             break;

    //             case render_command::command_type::draw_background:
    //             {
    //                 gfx::draw_background(&context, cmd);
    //             }
    //             break;

    //             case render_command::command_type::draw_mesh_with_color:
    //             {
    //                 gfx::draw_polygon_simple(&context,
    //                     cmd->draw_mesh_with_color.mesh_token, cmd->draw_mesh_with_color.shader_token,
    //                     cmd->draw_mesh_with_color.model, view, projection,
    //                     cmd->draw_mesh_with_color.color);
    //             }
    //             break;

    //             case render_command::command_type::draw_screen_frame:
    //             case render_command::command_type::draw_mesh_with_texture:
    //             {
    //                 // gfx::draw_polygon_simple(&context,
    //                 //     screen_frame_mesh, screen_frame_shader,
    //                 //     math::matrix4::identity(), math::matrix4::identity(), math::matrix4::identity(),
    //                 //     cmd->draw_screen_frame.color);
    //             }
    //             break;

    //             case render_command::command_type::draw_ui:
    //             {
    //                 gfx::draw_polygon_simple(&context,
    //                     cmd->draw_ui.mesh_token, cmd->draw_ui.shader_token,
    //                     cmd->draw_ui.model, cmd->draw_ui.view, cmd->draw_ui.projection,
    //                     cmd->draw_ui.color);
    //             }
    //             break;
    //         }
    //     }
    //     context.render_commands.clear();

    //     SDL_GL_SwapWindow(window);

    //     timepoint end_of_frame;
    //     end_of_frame.counts = SDL_GetTicks64();

    //     last_frame_dt =( (float32) (end_of_frame.counts - last_timepoint.counts)) / 1000000.f;
    //     last_timepoint = end_of_frame;
    // }

    // SDL_GL_DeleteContext(opengl_context);
    // SDL_DestroyWindow(window);

    // SDL_Quit();
    return 0;
}

// #include <gfx/renderer.cpp>
#include <memory/allocator.cpp>
#include <string_id.cpp>
// #include <rs/resource_system.cpp>

// #include <game_platformer/game.cpp>

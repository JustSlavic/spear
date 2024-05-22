#include <base.h>
#include <time.hpp>
#include <input.hpp>
#include <platform.hpp>
#include <gfx/renderer.hpp>
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


#include <ecs/entity_manager.hpp>


int main()
{
    auto global_memory = sdl::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));
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

    sdl::window window;
    sdl::create_opengl_window(800, 600, &window);

    gfx::driver driver = gfx::driver::initialize_opengl();

    driver.clear_color(0, 0, 0, 1);
    driver.depth_test(true);
    driver.write_depth(true);
    driver.vsync(true);

    // ======================================================================

    // ctx.aspect_ratio = 16.0f / 9.0f;
    // ctx.near_clip_dist = 0.05f;
    // ctx.near_clip_width = 2 * ctx.near_clip_dist * tanf(0.5f * to_radians(60));
    // ctx.near_clip_height = ctx.near_clip_width / ctx.aspect_ratio;
    // ctx.far_clip_dist = 100.f;
    // ctx.debug_load_file = NULL;

    // auto view = matrix4::identity();
    // auto projection = driver.make_projection_matrix_fov(to_radians(60), ctx.aspect_ratio, ctx.near_clip_dist, ctx.far_clip_dist);
    // auto projection_ui = matrix4::identity();

    // ======================================================================

    // auto manager = ecs::entity_manager::create();

    // auto eid1 = manager.create_entity();
    // auto eid2 = manager.create_entity();
    // auto eid3 = manager.create_entity();
    // auto eid4 = manager.create_entity();

    // printf("eid2 exists = %s\n", manager.is_entity_exists(eid2) ? "true" : "false");
    // manager.destroy_entity(eid2);
    // printf("eid2 exists = %s\n", manager.is_entity_exists(eid2) ? "true" : "false");

    // auto eid5 = manager.create_entity();
    // printf("eid = %d\n", eid1.id);

    // rs::storage rs;
    // memset(&rs, 0, sizeof(rs));

    // context ctx;
    // memset(&ctx, 0, sizeof(ctx));
    // ctx.temporary_allocator = temporary_arena;
    // ctx.resource_allocator = resource_allocator;
    // ctx.renderer_allocator = renderer_allocator;
    // ctx.strids = &string_id_storage;
    // ctx.rs = &rs;

    // // ======================================================================

    // execution_context context = {};
    // memory::initialize_memory_arena(&context.temporary_allocator, scratchpad_memory);
    // memory::initialize_memory_arena(&context.renderer_allocator, renderer_memory);
    // memory::initialize_memory_heap(&context.resource_storage.heap, resource_memory);

    // context.strid_storage = initialize_string_id_storage(string_id_memory);

    // context.execution_commands = ALLOCATE_ARRAY(&platform_allocator, execution_command, 5);
    // context.render_commands = ALLOCATE_ARRAY(&context.renderer_allocator, render_command, 1 << 12);
    // context.resource_storage.resources = ALLOCATE_ARRAY(&context.renderer_allocator, rs::resource, 32);
    // create_null_resource(&context.resource_storage); // Consider 0 resource being null-resource, indicating the lack of it.

    initialize_memory(&ctx, game_memory);

    // auto view = math::matrix4::identity();
    // float32 aspect_ratio = 16.0f / 9.0f;
    // auto projection = gfx::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    input_state input = {};

    // float32 last_frame_dt = 60.f;
    // timepoint last_timepoint;
    // last_timepoint.counts = SDL_GetTicks64();

    running = true;
    while (running)
    {
    //     reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
    //     reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
        process_pending_messages(&input);
    //     // sdl::get_mouse_pos(&window, &input.mouse.x, &input.mouse.y);
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

        driver.swap_buffers(&window);

    //     timepoint end_of_frame;
    //     end_of_frame.counts = SDL_GetTicks64();

    //     last_frame_dt =( (float32) (end_of_frame.counts - last_timepoint.counts)) / 1000000.f;
    //     last_timepoint = end_of_frame;
    }

    SDL_GL_DeleteContext(window.context);
    SDL_DestroyWindow(window.handle);

    SDL_Quit();
    return 0;
}

#include <gfx/renderer.cpp>
#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <rs/resource_system.cpp>
#include <image/png.cpp>
#include <game_stub.cpp>
#include <crc.cpp>
#include <os/platform_posix.cpp>
#include <ecs/entity_manager.cpp>
#include <collision.cpp>
#include <context.cpp>

#include <memory_bucket.cpp>

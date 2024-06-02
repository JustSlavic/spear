#include <base.h>
#include <time.hpp>
#include <input.hpp>
#include <platform.hpp>
#include <gfx/renderer.hpp>
#include <game_interface.hpp>
#include <platform_sdl.hpp>
#include <hash.hpp>


GLOBAL bool32 running;
GLOBAL int32 current_client_width;
GLOBAL int32 current_client_height;
GLOBAL bool32 viewport_changed;


void process_pending_messages(input_state *input)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type)
        {
            case SDL_QUIT:
                running = false;
            break;

            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
                    e.window.event == SDL_WINDOWEVENT_SHOWN ||
                    e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    viewport_changed = true;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                auto button = (e.button.button == 1) ? MOUSE_LEFT :
                              (e.button.button == 2) ? MOUSE_MIDDLE :
                              (e.button.button == 3) ? MOUSE_RIGHT : MOUSE_NONE;
                process_button_state(&input->mouse[button], e.button.state == SDL_PRESSED);
            }
            break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                auto key = sdl::map_button_from_scancode(e.key.keysym.scancode);
                if (key != KB_NONE)
                {
                    process_button_state(&input->keyboard[key], e.type == SDL_KEYDOWN);
                }
            }
            break;
        }
    }
}


#include <ecs/entity_manager.hpp>
#include <ecs/component.hpp>
#include <ecs/archetype.hpp>


void entity_system_1(ecs::event *evt, ecs::entity_id eid)
{
    printf("es: eid=%d\n", eid.id);
}

struct event_update : ecs::event {};


#define ECS_HASH hash_djb2


int main()
{
    auto global_memory = sdl::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));
    auto global_arena  = memory_allocator::make_arena(global_memory);

    auto game_memory = global_arena.allocate_buffer(MEGABYTES(5));
    auto resource_allocator = global_arena.allocate_arena(MEGABYTES(10));
    auto renderer_allocator = global_arena.allocate_arena(MEGABYTES(10));
    auto string_id_allocator = global_arena.allocate_arena(KILOBYTES(10));

    auto temporary_arena = global_arena.allocate_arena(MEGABYTES(1));
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
    if (driver.m_api != gfx::api::opengl)
    {
        printf("Error: Could not initialize opengl!\n");
        return -1;
    }

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
    ctx.debug_load_file = NULL;

    auto view = matrix4::identity();
    auto projection = driver.make_projection_matrix_fov(to_radians(60), ctx.aspect_ratio, ctx.near_clip_dist, ctx.far_clip_dist);
    auto projection_ui = matrix4::identity();

    // ======================================================================



    // ecs::component comps[] = {
    //     ECS_COMPONENT("eid", ecs::entity_id),
    //     ECS_COMPONENT("abc", int),
    //     ECS_COMPONENT("def", int),
    // };
    // ecs::archetype arch = ecs::make_archetype(comps, ARRAY_COUNT(comps));

    // auto eid1 = manager.create_entity();
    // auto comp_values = temporary_arena.allocate_buffer(sizeof(ecs::entity_id) + sizeof(int) * 2);
    // *(ecs::entity_id *)(comp_values.data) = eid1;
    // *(int *)(comp_values.data + sizeof(ecs::entity_id)) = 32;
    // *(int *)(comp_values.data + sizeof(ecs::entity_id) + sizeof(int)) = 42;
    // ecs::archetype_initializer init = { { comps[0], comps[1], comps[2] }, ARRAY_COUNT(comps), comp_values };
    // arch.allocate(init);

    // for (uint32 i = 0; i < arch.chunk.size / sizeof(int); i++)
    // {
    //     printf("%d ", *(int *)(arch.chunk.data + i*sizeof(int)));
    // }

    // // manager.register_event<event_update>(ECS_HASH("event_update"));
    // // manager.register_system<ECS_HASH("event_update")>(entity_system_1);
    // manager.send_event_immediate(event_update{});

    // ======================================================================

    initialize_memory(&ctx, game_memory);

    input_state input = {};

    float32 last_frame_dt = 0.016f;
    timepoint last_timepoint;
    last_timepoint.us = SDL_GetTicks64() * 1000;

    running = true;
    while (running)
    {
        reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
        reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
        process_pending_messages(&input);
        sdl::get_mouse_pos(&input.mouse.x, &input.mouse.y);
        input.dt = last_frame_dt;
        input.time = last_timepoint;

        if (viewport_changed)
        {
            SDL_GetWindowSize(window.handle, &current_client_width, &current_client_height);
            ctx.viewport = gfx::viewport::make(current_client_width, current_client_height, ctx.aspect_ratio);
            driver.set_viewport(ctx.viewport);
            viewport_changed = false;

            ctx.window_width = current_client_width;
            ctx.window_height = current_client_height;

            projection_ui =
                matrix4::translate(-1, 1, 0) *
                matrix4::scale(2.0f/ctx.viewport.width, -2.0f/ctx.viewport.height, 1);
        }

        driver.clear();
        update_and_render(&ctx, game_memory, &input);

        for (auto cmd : ctx.exec_commands)
        {
            switch (cmd.kind)
            {
                case exec_command::exit:
                {
                    running = false;
                }
                break;
            }
        }
        ctx.exec_commands.clear();

        for (auto cmd : ctx.rend_commands)
        {
            if (cmd.kind == rend_command::setup_camera)
            {
                view = driver.make_look_at_matrix(cmd.position, cmd.position + cmd.forward, cmd.up);
            }
            else if (cmd.kind == rend_command::render_mesh_single_color)
            {
                driver.render_mesh_single_color(&ctx, cmd.model, view, projection, cmd.mesh_token, cmd.shader_token, cmd.color);
            }
            else if (cmd.kind == rend_command::render_ui_single_color)
            {
                driver.write_depth(false);
                driver.render_mesh_single_color(&ctx, cmd.model, matrix4::identity(), projection_ui, cmd.mesh_token, cmd.shader_token, cmd.color);
                driver.write_depth(true);
            }
            // else if (cmd.kind == rend_command::render_mesh_texture)
            // {
            //     driver.write_depth(false);
            //     driver.render_mesh_texture(&ctx, cmd.model, view, projection, cmd.mesh_token, cmd.shader_token, cmd.texture_token);
            //     driver.write_depth(true);
            // }
            // else if (cmd.kind == rend_command::render_text)
            // {
            //     driver.write_depth(false);
            //     driver.render_text(&ctx, projection_ui, cmd.mesh_token, cmd.shader_token, cmd.texture_token, cmd.text, cmd.color);
            //     driver.write_depth(true);
            // }
        }
        ctx.rend_commands.clear();

        driver.swap_buffers(&window);

        timepoint end_of_frame;
        end_of_frame.us = SDL_GetTicks64() * 1000;

        last_frame_dt = get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
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
#include <game/game.cpp>
#include <crc.cpp>
#include <os/platform_posix.cpp>
#include <ecs/entity_manager.cpp>
#include <collision.cpp>
#include <context.cpp>

#include <memory_bucket.cpp>
#include <ecs/archetype.cpp>

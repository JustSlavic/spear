#include <base.h>
#include <time.hpp>
#include <input.hpp>
#include <game_interface.hpp>
#include <platform_sdl.hpp>
#include <hash.hpp>

#include <gfx/viewport.hpp>
#include <gfx/static_shaders.cpp>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <common_graphics.hpp>


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


timepoint now()
{
    timepoint result = { SDL_GetTicks64() * 1000 };
    return result;
}


int main()
{
    current_client_width = 800;
    current_client_height = 450;

    auto global_memory = sdl::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));
    auto global_arena  = memory_allocator::make_arena(global_memory);

    auto game_memory = global_arena.allocate_buffer(MEGABYTES(5));

    // ======================================================================

    int err = SDL_Init(SDL_INIT_VIDEO);
    if (err < 0)
    {
        printf("Could not initialize SDL2: \"%s\"\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    auto window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        current_client_width, current_client_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
        printf("Could not create SDL Window: \"%s\"\n", SDL_GetError());
        exit(2);
    }

    auto render_context = SDL_GL_CreateContext(window);
    if (render_context == NULL)
    {
        printf("Could not create OpenGL context: \"%s\"\n", SDL_GetError());
        exit(4);
    }

    SDL_GL_MakeCurrent(window, render_context);

    SDL_version compiled;
    SDL_VERSION(&compiled);
    printf("Compiled against SDL2 v.%d.%d.%d\n", (int)compiled.major, (int)compiled.minor, (int)compiled.patch);

    SDL_version linked;
    SDL_GetVersion(&linked);
    printf("Linked against SDL2 v.%d.%d.%d\n", (int)linked.major, (int)linked.minor, (int)linked.patch);

    printf("glGetString(GL_SHADING_LANGUAGE_VERSION) -> %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLubyte const *v = glGetString(GL_VERSION);
    printf("glGetString(GL_VERSION) -> %s\n", v);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);
    
    // ======================================================================

    context ctx;
    memset(&ctx, 0, sizeof(ctx));

    ctx.aspect_ratio = 16.0f / 9.0f;
    ctx.near_clip_dist = 0.05f;
    ctx.near_clip_width = 2 * ctx.near_clip_dist * tanf(0.5f * to_radians(60));
    ctx.near_clip_height = ctx.near_clip_width / ctx.aspect_ratio;
    ctx.far_clip_dist = 100.f;
    ctx.debug_load_file = NULL;

    // ======================================================================

    auto view_matrix = matrix4::identity();
    auto proj_matrix = make_projection_matrix_fov(to_radians(60), ctx.aspect_ratio, ctx.near_clip_dist, ctx.far_clip_dist);
    auto proj_matrix_ui = matrix4::identity();

    // ======================================================================

    auto cpu_square = make_square();
    auto gpu_square = load_mesh(cpu_square);

    auto cpu_cube = make_cube();
    auto gpu_cube = load_mesh(cpu_cube);

    auto shader_color = compile_shaders(vs_single_color, fs_pass_color);
    auto shader_ground = compile_shaders(vs_ground, fs_pass_color);
    auto shader_framebuffer = compile_shaders(vs_framebuffer, fs_framebuffer);



    // ======================================================================

    // auto ui_render_target = driver.create_render_target(&ctx);

    // driver.use_render_target(ui_render_target);
    // driver.clear_color(1.f, 1.f, 0.f, 1.f);
    // driver.clear();

    // ======================================================================

    auto ui_framebuffer = create_framebuffer(current_client_width, current_client_height);

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
    timepoint last_timepoint = now();

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
            SDL_GetWindowSize(window, &current_client_width, &current_client_height);
            auto viewport = gfx::viewport::make(current_client_width, current_client_height, ctx.aspect_ratio);
            glViewport(viewport.offset_x, viewport.offset_y, viewport.width, viewport.height);
            viewport_changed = false;

            ctx.viewport = viewport;

            printf("Viewport size = %d x %d\n", viewport.width, viewport.height);

            ctx.window_width = current_client_width;
            ctx.window_height = current_client_height;

            proj_matrix_ui = matrix4::translate(-1, 1, 0)
                           * matrix4::scale(2.0f/viewport.width, -2.0f/viewport.height, 1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, ui_framebuffer.framebuffer_id);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto cmd : ctx.rend_commands)
        {
            if (cmd.kind == rend_command::setup_camera)
            {
                view_matrix = make_lookat_matrix(cmd.position, cmd.position + cmd.forward, cmd.up);
            }
            else if (cmd.kind == rend_command::render_square)
            {
                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", cmd.model);
                shader_color.uniform("u_view", view_matrix);
                shader_color.uniform("u_projection", proj_matrix);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_square.ibo);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.kind == rend_command::render_cube)
            {
                shader *s = NULL;
                if (cmd.shader == SHADER_COLOR)
                    s = &shader_color;
                else if (cmd.shader == SHADER_GROUND)
                    s = &shader_ground;
                else
                    ASSERT_FAIL();

                glUseProgram(s->id);

                s->uniform("u_model", cmd.model);
                s->uniform("u_view", view_matrix);
                s->uniform("u_projection", proj_matrix);
                s->uniform("u_color", cmd.color);

                glBindVertexArray(gpu_cube.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_cube.ibo);
                glDrawElements(GL_TRIANGLES, gpu_cube.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.kind == rend_command::render_ui)
            {
                ASSERT_FAIL();
            }
        }
        ctx.rend_commands.clear();

        glBindFramebuffer(GL_FRAMEBUFFER, ui_framebuffer.framebuffer_id);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto cmd : ctx.rend_commands_ui)
        {
            if (cmd.kind == rend_command::render_ui)
            {
                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", cmd.model);
                shader_color.uniform("u_view", matrix4::identity());
                shader_color.uniform("u_projection", proj_matrix_ui);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.kind == rend_command::render_banner)
            {
                auto pNDC = proj_matrix * view_matrix * matrix4::translate(cmd.position) * V4(0, 0, 0, 1);
                pNDC /= pNDC.w;
                auto pUI = matrix4::scale(0.5f * ctx.viewport.width, - 0.5f * ctx.viewport.height, 1)
                         * matrix4::translate(1, -1, 0)
                         * pNDC;
                auto m = matrix4::translate(pUI.xyz) * cmd.model;

                // proj_ui * translate(pUI) * scale * p

                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", m);
                shader_color.uniform("u_view", matrix4::identity());
                shader_color.uniform("u_projection", proj_matrix_ui);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else
            {
                ASSERT_FAIL();
            }
        }
        ctx.rend_commands_ui.clear();

        // Draw UI on top of everything
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(shader_framebuffer.id);
            shader_framebuffer.uniform("u_framebuffer", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ui_framebuffer.color_texture_id);

            glBindVertexArray(gpu_square.vao);
            glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);

            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }

        SDL_GL_SwapWindow(window);

        timepoint end_of_frame = now();
        last_frame_dt = get_seconds(end_of_frame - last_timepoint);
        last_timepoint = end_of_frame;
    }

    SDL_GL_DeleteContext(render_context);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <game/game.cpp>
#include <crc.cpp>
#include <os/platform_posix.cpp>
#include <ecs/entity_manager.cpp>
#include <collision.cpp>
#include <context.cpp>

#include <memory_bucket.cpp>
#include <ecs/archetype.cpp>

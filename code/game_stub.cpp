#include "game_stub.hpp"
#include "game_interface.hpp"

#include <gfx/vertex_buffer_layout.hpp>



INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_buffer game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);
    auto arena = memory_allocator::make_arena(game_memory);

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

    // Init rectangle mesh
    {
        float32 vbo_data[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
        };
        memory_buffer vbo;
        vbo.data = (byte *) vbo_data;
        vbo.size = sizeof(vbo_data);

        uint32 ibo_data[] = {
            0, 1, 2, // first triangle
            2, 3, 0, // second triangle
        };
        memory_buffer ibo;
        ibo.data = (byte *) ibo_data;
        ibo.size = sizeof(ibo_data);

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        gs->rect_mesh = rs::create_mesh(ctx->rs, vbo, ibo, vbl);
    }

    // 3D cube
    {
        float32 vbo_data[] = {
            // bottom square
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
            // top square
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
        };
        memory_buffer vbo;
        vbo.data = (byte *) vbo_data;
        vbo.size = sizeof(vbo_data);

        uint32 ibo_data[] = {
            0, 1, 2,
            2, 3, 0,

            0, 3, 7,
            7, 4, 0,

            1, 5, 6,
            6, 2, 1,

            3, 2, 6,
            6, 7, 3,

            1, 0, 4,
            4, 5, 1,

            5, 4, 7,
            7, 6, 5,
        };
        memory_buffer ibo;
        ibo.data = (byte *) ibo_data;
        ibo.size = sizeof(ibo_data);

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        gs->cube_mesh = rs::create_mesh(ctx->rs, vbo, ibo, vbl);
    }

    gs->the_only_shader = rs::create_shader(ctx->rs);

    gs->camera = game::camera::look_at(V3(0, -5, 10), V3(0, 0, 0), V3(0, 0, 1));
    gs->camera_speed = 2.f;
}


UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_buffer game_memory, input_state *input)
{
    auto gs = (game_state *) ctx->game_state;
    auto dt = input->dt;

    if (get_release_count(input->keyboard[KB_ESC]))
    {
        if (get_seconds(input->time - gs->exit_press_time) < 1)
        {
            ctx->exit_game();
        }
        else
        {
            gs->exit_press_time = input->time;
        }
    }

    // Move camera
    {
        auto camera_move_direction = V3(0, 0, 0);
        if (get_hold_count(input->keyboard[KB_A])) camera_move_direction -= V3(1, 0, 0) * dt;
        if (get_hold_count(input->keyboard[KB_D])) camera_move_direction += V3(1, 0, 0) * dt;
        if (get_hold_count(input->keyboard[KB_W])) camera_move_direction += V3(0, 1, 0) * dt;
        if (get_hold_count(input->keyboard[KB_S])) camera_move_direction -= V3(0, 1, 0) * dt;
        if (get_hold_count(input->keyboard[KB_R])) camera_move_direction += V3(0, 0, 1) * dt;
        if (get_hold_count(input->keyboard[KB_F])) camera_move_direction -= V3(0, 0, 1) * dt;
        gs->camera.position += normalized(camera_move_direction) * gs->camera_speed * dt;
    }

    ctx->setup_camera(gs->camera.position, gs->camera.forward, gs->camera.up);

    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto m = matrix4::translate_x((float32)x) *
                     matrix4::translate_y((float32)y) *
                     matrix4::scale(0.4f);
            ctx->render_mesh(m, gs->cube_mesh, gs->the_only_shader, V4(0.3 - 0.1f * x, 0 + 0.2f * y, 0.3 + 0.1f * x, 1));
        }
    }
}


#include <context.cpp>
#include <memory_bucket.cpp>
#include <memory_allocator.cpp>
#include <rs/resource_system.cpp>

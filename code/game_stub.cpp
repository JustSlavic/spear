#include "game_stub.hpp"
#include "game_interface.hpp"

#include <math/float32.h>

#include <gfx/vertex_buffer_layout.hpp>

#include <math/projective_geometry3.hpp>
#include <collision.hpp>

#include <math/rectangle3.hpp>

#include <game/systems.hpp>


#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))


INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_buffer game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);
    auto arena = memory_allocator::make_arena(game_memory);

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

    // Rectangle mesh
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

        auto vbl = gfx::vertex_buffer_layout::make();
        vbl.push<float>(3);

        gs->rect_mesh = rs::create_mesh(ctx, ctx->rs, vbo, ibo, vbl);
    }

    // Rectangle mesh with UV
    {
        float32 vbo_data[] = {
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
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

        auto vbl = gfx::vertex_buffer_layout::make();
        vbl.push<float>(3);
        vbl.push<float>(2);

        gs->rect_mesh_uv = rs::create_mesh(ctx, ctx->rs, vbo, ibo, vbl);
    }

    // Buffers for text
    {
        auto vbo = memory_buffer::make();
        auto ibo = memory_buffer::make();
        auto vbl = gfx::vertex_buffer_layout::make();
        vbl.push<float>(2);
        vbl.push<float>(2);

        gs->text_buffers = rs::create_mesh(ctx, ctx->rs, vbo, ibo, vbl);
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

        auto vbl = gfx::vertex_buffer_layout::make();
        vbl.push<float>(3);

        gs->cube_mesh = rs::create_mesh(ctx, ctx->rs, vbo, ibo, vbl);
    }

    gs->shader_single_color = rs::create_shader(ctx, ctx->rs, string_id::from(ctx->strids, "SHADER_SINGLE_COLOR"));
    gs->shader_draw_texture = rs::create_shader(ctx, ctx->rs, string_id::from(ctx->strids, "SHADER_DRAW_TEXTURE"));
    gs->shader_draw_text = rs::create_shader(ctx, ctx->rs, string_id::from(ctx->strids, "SHADER_DRAW_TEXT"));

    // gs->font_texture = rs::load_texture(ctx, ctx->rs, "font.png");

    gs->camera = game::camera::look_at(V3(0, -15, 15), V3(0, 0, 0), V3(0, 0, 1));
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

    game::move_camera(gs, input);
    ctx->setup_camera(gs->camera.position, gs->camera.forward, gs->camera.up);

    vector3 ray_direction = game::compute_pointer_ray(ctx, gs, input);
    vector3 intersection;
    {
        auto Oxy = make_plane3(0, 0, 1, 0);
        auto line = make_line3(ray_direction, gs->camera.position);
        auto intersection_p = outer(Oxy, line);
        intersection = intersection_p.vector / intersection_p.w;
    }

    // printf("mouse = (%d, %d)\n", input->mouse.x, input->mouse.y);
    // printf("ray_direction = (%f, %f, %f)\n", ray_direction.x, ray_direction.y, ray_direction.z);

    bool32 intersected = false;
    float32 intersect_t = infinity;
    int intersect_x = 0, intersect_y = 0;
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto center = V3(x + 1.3f*x, y + 1.3f*y, 0);
            rectangle3 aabb = rectangle3::from_center_radius(center, 1, 1, 1);

            float tmin;
            int intersect_cube = intersect_ray_aabb(gs->camera.position, ray_direction, aabb, &tmin);

            if (intersect_cube && tmin < intersect_t)
            {
                intersected = true;
                intersect_t = tmin;
                intersect_x = x;
                intersect_y = y;
            }
        }
    }

    ctx->render_ui(matrix4::identity(), gs->rect_mesh, gs->shader_single_color, V4(0.0, 7.0, 9.0, 1.0));

    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto c = V4(0.8 - 0.1f * x, 0.5 + 0.1f * y, 0.3 + 0.1f * x, 1);
            if (intersected && x == intersect_x && y == intersect_y) c = V4(1, 0, 0, 1);

            auto m = matrix4::translate_x((float32)x + 1.3f*x) *
                     matrix4::translate_y((float32)y + 1.3f*y);
            ctx->render_mesh(m, gs->cube_mesh, gs->shader_single_color, c);
        }
    }

    // ctx->render_text(gs->font_texture, gs->text_buffers, gs->shader_draw_text, string_view::from("Lorem ipsum dolor sit amet"), V4(1));
}

#if DLL_BUILD
#include <context.cpp>
#include <string_id.cpp>
#include <memory_bucket.cpp>
#include <memory/allocator.cpp>
#include <rs/resource_system.cpp>
#include <collision.cpp>
#include <image/png.cpp>
#include <crc.cpp>
#endif // DLL_BUILD

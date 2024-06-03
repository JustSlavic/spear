#include "game.hpp"
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
    gs->shader_ground = rs::create_shader(ctx, ctx->rs, string_id::from(ctx->strids, "SHADER_DRAW_GROUND"));

    // gs->font_texture = rs::load_texture(ctx, ctx->rs, "font.png");

    gs->camera = game::camera::look_at(V3(0, -15, 15), V3(0, 0, 0), V3(0, 0, 1));
    gs->camera_speed = 2.f;

    gs->seconds_for_turn = duration::seconds(5);

    // Init ECS
    gs->entity_manager = ecs::entity_manager::create();

    game::spawn_hero(gs, 0, 0);
    game::spawn_monster(gs, 1, 1);
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

    entity *hero = gs->entities + gs->hero_id.get_index();

    if (get_press_count(input->keyboard[KB_SPACE]) ||
        (input->time >= (gs->turn_start_time + gs->seconds_for_turn)))
    {
        // @attention NEW TURN !!!

        game::apply_entity_action(gs, hero);
        gs->turn_start_time = input->time;
        hero->action = game::null_action();

        game::apply_monster_actions(gs);
    }

    int move_to_x = hero->x;
    int move_to_y = hero->y;

    if (intersected && get_press_count(input->mouse[MOUSE_LEFT]))
    {
        move_to_x = intersect_x;
        move_to_y = intersect_y;
    }
    if (get_press_count(input->keyboard[KB_W]))
    {
        move_to_x = hero->x;
        move_to_y = hero->y + 1;
    }
    if (get_press_count(input->keyboard[KB_A]))
    {
        move_to_x = hero->x - 1;
        move_to_y = hero->y;
    }
    if (get_press_count(input->keyboard[KB_S]))
    {
        move_to_x = hero->x;
        move_to_y = hero->y - 1;
    }
    if (get_press_count(input->keyboard[KB_D]))
    {
        move_to_x = hero->x + 1;
        move_to_y = hero->y;
    }
    if (get_press_count(input->keyboard[KB_Q]))
    {
        gs->action_input.kind = ENTITY_ACTION_LEFT_ARM;
        TOGGLE(gs->selecting_direction_of_action);
    }
    if (get_press_count(input->keyboard[KB_E]))
    {
        gs->action_input.kind = ENTITY_ACTION_RIGHT_ARM;
        TOGGLE(gs->selecting_direction_of_action);
    }

    if (game::cell_on_board(move_to_x, move_to_y) &&
        game::cell_is_adjacent_to_entity(hero, move_to_x, move_to_y))
    {
        gs->action_input.x = move_to_x;
        gs->action_input.y = move_to_y;
        if (!gs->selecting_direction_of_action)
        {
            gs->action_input.kind = ENTITY_ACTION_MOVE;
        }

        if (gs->action_input.kind == ENTITY_ACTION_MOVE)
        {
            if (game::entity_can_walk_here(gs, hero, gs->action_input.x, gs->action_input.y))
            {
                hero->action = gs->action_input;
                gs->selecting_direction_of_action = false;
            }
        }
        else if (gs->action_input.kind != ENTITY_ACTION_NONE)
        {
            hero->action = gs->action_input;
            gs->selecting_direction_of_action = false;
        }
    }

    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto c = V4(0.8 - 0.1f * x, 0.5 + 0.1f * y, 0.3 + 0.1f * x, 1);

            if (gs->selecting_direction_of_action)
            {
                if (game::cell_is_adjacent_to_entity(hero, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }
            }
            else
            {
                if (game::entity_can_walk_here(gs, hero, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }

                if (x == hero->action.x &&
                    y == hero->action.y)
                {
                    if (hero->action.kind == ENTITY_ACTION_MOVE)
                        c = V4(0.4, 0.4, 0.8, 1);
                    else if (hero->action.kind == ENTITY_ACTION_LEFT_ARM)
                        c = V4(0.2, 0.6, 0.2, 1);
                    else if (hero->action.kind == ENTITY_ACTION_RIGHT_ARM)
                        c = V4(0.8, 0.2, 0.2, 1);
                }
            }

            // if (game::cell_is_adjacent_to_entity(hero, x, y))
            // {
            //     if (!gs->selecting_direction_of_action &&
            //         hero->action.kind != ENTITY_ACTION_NONE &&
            //         x == hero->action.x &&
            //         y == hero->action.y)
            //     {
            //         if (hero->action.kind == ENTITY_ACTION_LEFT_ARM)
            //             c = V4(0.2, 0.6, 0.2, 1);
            //         else if (hero->action.kind == ENTITY_ACTION_RIGHT_ARM)
            //             c = V4(0.8, 0.2, 0.2, 1);
            //         else
            //             c = V4(0.4, 0.4, 0.8, 1);
            //     }
            // }

            if (intersected && x == intersect_x && y == intersect_y)
            {
                c += V4(0.3, 0.3, 0.3, 1);
            }

            auto m = matrix4::translate_x((float32)x + 1.3f*x) *
                     matrix4::translate_y((float32)y + 1.3f*y);
            ctx->render_mesh(m, gs->cube_mesh, gs->shader_ground, c);
        }
    }

    // Draw hero
    {
        auto m = matrix4::translate_x((float32) hero->x + 1.3f*hero->x) *
                 matrix4::translate_y((float32) hero->y + 1.3f*hero->y) *
                 matrix4::translate_z(2) *
                 matrix4::scale(0.5f, 0.5f, 0.8f);
        ctx->render_mesh(m, gs->cube_mesh, gs->shader_single_color, V4(1, 1, 1, 1));
    }

    // Draw monsters
    {
        for (int i = 0; i < gs->monster_count; i++)
        {
            entity *monster = gs->entities + gs->monsters[i].get_index();
            auto m = matrix4::translate_x((float32) monster->x + 1.3f*monster->x) *
                     matrix4::translate_y((float32) monster->y + 1.3f*monster->y) *
                     matrix4::translate_z(2) *
                     matrix4::scale(0.5f, 0.5f, 0.3f);
            ctx->render_mesh(m, gs->cube_mesh, gs->shader_single_color, V4(1, 0.2, 0.1, 1));
        }
    }

    {
        auto color_left_arm = V4(0.2, 0.5, 0.7, 1.0);
        auto color_torso = V4(0.2, 0.5, 0.7, 1.0);
        auto color_right_arm = V4(0.2, 0.5, 0.7, 1.0);

        if ((!gs->selecting_direction_of_action && hero->action.kind == ENTITY_ACTION_LEFT_ARM) ||
            (gs->selecting_direction_of_action && gs->action_input.kind == ENTITY_ACTION_LEFT_ARM))
            color_left_arm += V4(0.5, 0.2, 0.1, 0);
        if ((!gs->selecting_direction_of_action && hero->action.kind == ENTITY_ACTION_RIGHT_ARM) ||
            (gs->selecting_direction_of_action && gs->action_input.kind == ENTITY_ACTION_RIGHT_ARM))
            color_right_arm += V4(0.5, 0.2, 0.1, 0);

        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(25) *
                       matrix4::scale(25, 50, 1)
            , gs->rect_mesh, gs->shader_single_color, color_left_arm);
        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(80) *
                       matrix4::scale(25, 50, 1)
            , gs->rect_mesh, gs->shader_single_color, color_torso);
        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(135) *
                       matrix4::scale(25, 50, 1)
            , gs->rect_mesh, gs->shader_single_color, color_right_arm);
    }

    {
        float32 t = 1 - get_seconds(input->time - gs->turn_start_time) / get_seconds(gs->seconds_for_turn);
        vector4 color = V4(sin((t - 3) * pi * 0.5f),
                           -cos((t + 1) * pi * 0.5f),
                           0, 1);
        ctx->render_ui(
                       matrix4::translate_y(ctx->viewport.height - 10) *
                       matrix4::scale(ctx->viewport.width * t, 2, 1)
            , gs->rect_mesh, gs->shader_single_color, color);
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

#include <ecs/entity_id.cpp>

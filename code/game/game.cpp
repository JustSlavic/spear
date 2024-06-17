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


bool game_state::is_coords_valid(int x, int y)
{
    return (-2 <= x && x <= 2) && (-2 <= y && y <= 2);
}

ecs::entity_id game_state::get_map_eid(int x, int y)
{
    ecs::entity_id result = ecs::INVALID_ENTITY_ID;
    if (is_coords_valid(x, y))
    {
        result = map[x + 2][y + 2];
    }
    return result;
}

void game_state::set_map_eid(int x, int y, ecs::entity_id eid)
{
    if (is_coords_valid(x, y))
    {
        map[x + 2][y + 2] = eid;
    }
}

INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_buffer game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);
    auto arena = memory_allocator::make_arena(game_memory);

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

    memset(gs->map, 0, sizeof(ecs::entity_id) * 5 * 5);

    gs->camera = game::camera::look_at(V3(0, -15, 15), V3(0, 0, 0), V3(0, 0, 1));
    gs->camera_speed = 2.f;

    gs->turn_no = 1;
    gs->turn_timer_enabled = false;
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

    entity *selected_entity = game::get_entity(gs, gs->selected_entity_eid);
    entity *hero = game::get_entity(gs, gs->hero_eid);

    bool force_new_turn = get_press_count(input->keyboard[KB_SPACE]);
    bool timer_new_turn = gs->turn_timer_enabled && (input->time >= (gs->turn_start_time + gs->seconds_for_turn));

    if (force_new_turn || timer_new_turn)
    {
        // @attention NEW TURN !!!
        if (gs->hero_eid == ecs::INVALID_ENTITY_ID)
        {
            if (gs->spawn_hero_next_turn)
            {
                if (game::cell_is_empty(gs, 0, 0))
                    game::spawn_hero(gs, 0, 0);
            }
            else
            {
                gs->spawn_hero_next_turn = true;
            }
        }

        gs->action_buffer.clear();
        if (hero)
            gs->action_buffer.push_back(game::get_action2(hero));

        for (int i = 0; i < gs->monsters.size(); i++)
        {
            entity *monster = game::get_entity(gs, gs->monsters[i]);
            gs->action_buffer.push_back(game::get_action2(monster));
        }

        // Cancel intersecting moves
        for (int i = 0; i < gs->action_buffer.size(); i++)
        {
            auto *a1 = &gs->action_buffer[i];
            for (int j = i + 1; j < gs->action_buffer.size(); j++)
            {
                auto *a2 = &gs->action_buffer[j];
                if (a1->kind == ENTITY_ACTION2_MOVE && a2->kind == ENTITY_ACTION2_MOVE)
                {
                    if (a1->x1 == a2->x1 && a1->y1 == a2->y1)
                    {
                        // a1 -> <- a2
                        a1->kind = ENTITY_ACTION2_NONE;
                        a2->kind = ENTITY_ACTION2_NONE;
                    }
                    else if (a1->x1 == a2->x0 && a1->y1 == a2->y0)
                    {
                        // a1 -> a2 ->
                        ASSERT_FAIL("Should not be possible");
                    }
                    else if (a1->x0 == a2->x1 && a1->y0 == a2->y1)
                    {
                        // <- a1 <- a2
                        ASSERT_FAIL("Should not be possible");
                    }
                }
            }
        }

        // Remove null actions
        {
            int i = (int) gs->action_buffer.size();
            while (i-->0)
            {
                if (gs->action_buffer[i].kind == ENTITY_ACTION2_NONE)
                {
                    entity *e = game::get_entity(gs, gs->action_buffer[i].eid);
                    e->action = game::null_action();

                    gs->action_buffer.erase_not_sorted(i);
                }
            }
        }

        for (int i = 0; i < gs->action_buffer.size(); i++)
        {
            auto *a1 = &gs->action_buffer[i];
            for (int j = i + 1; j < gs->action_buffer.size(); j++)
            {
                auto *a2 = &gs->action_buffer[j];
                if (a1->kind == ENTITY_ACTION2_MOVE && a2->kind == ENTITY_ACTION2_ATTACK)
                {
                    if (a1->x0 == a2->x1 && a1->y0 == a2->y1)
                    {
                        // a2 --a-> a1 ->
                        auto tmp = *a1;
                        *a1 = *a2;
                        *a2 = tmp;
                    }
                    else if (a1->x1 == a2->x1 && a1->y1 == a2->y1)
                    {
                        // a2 --a->  <- a1
                        // All right
                    }
                }
                else if (a1->kind == ENTITY_ACTION2_ATTACK && a2->kind == ENTITY_ACTION2_MOVE)
                {
                    if (a1->x0 == a2->x1 && a1->y0 == a2->y1)
                    {
                        // a2 --a-> a1 ->
                        // All right
                    }
                    else if (a1->x1 == a2->x1 && a1->y1 == a2->y1)
                    {
                        // a2 --a->  <- a1
                        auto tmp = *a1;
                        *a1 = *a2;
                        *a2 = tmp;
                    }
                }
                else if (a1->kind == ENTITY_ACTION2_ATTACK && a2->kind == ENTITY_ACTION2_DEFENCE)
                {
                    if ((a1->x1 == a2->x0 && a1->y1 == a2->y0) &&
                        (a1->x0 == a2->x1 && a1->y0 == a2->y1))
                    {
                        // a1 --a-> a2
                        //    <-d--
                        auto tmp = *a1;
                        *a1 = *a2;
                        *a2 = tmp;
                    }
                }
            }
        }

        game::apply_actions(gs);
        game::remove_dead_entities(gs);
        game::reset_entity_states(gs);

        gs->turn_no += 1;
        gs->turn_start_time = input->time;
    }

    if (intersected && gs->get_map_eid(intersect_x, intersect_y) != ecs::INVALID_ENTITY_ID &&
        !game::entity_can_walk_here(gs, selected_entity, intersect_x, intersect_y) &&
        !gs->selecting_direction_of_action)
    {
        if (get_press_count(input->mouse[MOUSE_LEFT]))
        {
            gs->selected_entity_eid = gs->get_map_eid(intersect_x, intersect_y);
            selected_entity = game::get_entity(gs, gs->selected_entity_eid);
        }
    }

    if (intersected)
    {
        if (game::cell_is_empty(gs, intersect_x, intersect_y))
        {
            if (get_press_count(input->keyboard[KB_P]))
            {
                game::spawn_monster(gs, intersect_x, intersect_y);
            }
        }
    }

    if (selected_entity)
    {
        int move_to_x = selected_entity->x;
        int move_to_y = selected_entity->y;

        if (intersected && get_press_count(input->mouse[MOUSE_LEFT]))
        {
            move_to_x = intersect_x;
            move_to_y = intersect_y;
        }
        if (get_press_count(input->keyboard[KB_W]))
        {
            move_to_x = selected_entity->x;
            move_to_y = selected_entity->y + 1;
        }
        if (get_press_count(input->keyboard[KB_A]))
        {
            move_to_x = selected_entity->x - 1;
            move_to_y = selected_entity->y;
        }
        if (get_press_count(input->keyboard[KB_S]))
        {
            move_to_x = selected_entity->x;
            move_to_y = selected_entity->y - 1;
        }
        if (get_press_count(input->keyboard[KB_D]))
        {
            move_to_x = selected_entity->x + 1;
            move_to_y = selected_entity->y;
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

        if (gs->is_coords_valid(move_to_x, move_to_y) &&
            game::cell_is_adjacent_to_entity(selected_entity, move_to_x, move_to_y))
        {
            gs->action_input.x = move_to_x;
            gs->action_input.y = move_to_y;
            if (!gs->selecting_direction_of_action)
            {
                gs->action_input.kind = ENTITY_ACTION_MOVE;
            }

            if (gs->action_input.kind == ENTITY_ACTION_MOVE)
            {
                if (game::entity_can_walk_here(gs, selected_entity, gs->action_input.x, gs->action_input.y))
                {
                    selected_entity->action = gs->action_input;
                    gs->selecting_direction_of_action = false;
                }
            }
            else if (gs->action_input.kind != ENTITY_ACTION_NONE)
            {
                selected_entity->action = gs->action_input;
                gs->selecting_direction_of_action = false;
            }
        }
    }

    // Render ground
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto c = V4(0.8 - 0.1f * x, 0.5 + 0.1f * y, 0.3 + 0.1f * x, 1);

            if (gs->selecting_direction_of_action)
            {
                if (game::cell_is_adjacent_to_entity(selected_entity, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }
            }
            else if (selected_entity)
            {
                if (game::entity_can_walk_here(gs, selected_entity, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }

                if (x == selected_entity->action.x &&
                    y == selected_entity->action.y)
                {
                    if (selected_entity->action.kind == ENTITY_ACTION_MOVE)
                        c = V4(0.4, 0.4, 0.8, 1);
                    else if (selected_entity->action.kind == ENTITY_ACTION_LEFT_ARM)
                        c = V4(0.2, 0.6, 0.2, 1);
                    else if (selected_entity->action.kind == ENTITY_ACTION_RIGHT_ARM)
                        c = V4(0.8, 0.2, 0.2, 1);
                }
            }

            if (intersected && x == intersect_x && y == intersect_y)
            {
                c += V4(0.3, 0.3, 0.3, 1);
            }

            auto m = matrix4::translate_x((float32)x + 1.3f*x) *
                     matrix4::translate_y((float32)y + 1.3f*y);

            ctx->render_cube(m, c, SHADER_GROUND);
        }
    }

    float selected_entity_height = 0.8f;
    float regular_entity_height = 0.3f;

    // Draw hero
    if (hero)
    {
        float32 x = hero->x + 1.3f*hero->x;
        float32 y = hero->y + 1.3f*hero->y;
        float height = hero->eid == gs->selected_entity_eid ? selected_entity_height
                     : regular_entity_height;
        auto m = matrix4::translate(x, y, 2) *
                 matrix4::scale(0.5f, 0.5f, height);

        ctx->render_cube(m, V4(1, 1, 1, 1), SHADER_COLOR);

        // Render hp
        {
            auto mHP = m;

            for (int i = 0; i < hero->hp; i++)
            {
                auto color = V4(1, 0.2, 0.1, 1);
                ctx->render_banner(V3(x - 1 + i, y + 1, 2), matrix4::scale(10, 10, 1)
                    , color);
            }
        }
    }

    // Draw monsters
    {
        for (int i = 0; i < gs->monsters.size(); i++)
        {
            entity *monster = game::get_entity(gs, gs->monsters[i]);
            float height = monster->eid == gs->selected_entity_eid ? selected_entity_height
                         : regular_entity_height;

            auto m = matrix4::translate_x((float32) monster->x + 1.3f*monster->x) *
                     matrix4::translate_y((float32) monster->y + 1.3f*monster->y) *
                     matrix4::translate_z(2) *
                     matrix4::scale(0.5f, 0.5f, height);
            ctx->render_cube(m, V4(1, 0.2, 0.1, 1), SHADER_COLOR);
        }
    }

    // Draw character page
    if (hero)
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
            , color_left_arm);
        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(80) *
                       matrix4::scale(25, 50, 1)
            , color_torso);
        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(135) *
                       matrix4::scale(25, 50, 1)
            , color_right_arm);
    }

    // Render timer
    if (gs->turn_timer_enabled)
    {
        float32 t = (float32) (1.f - get_seconds(input->time - gs->turn_start_time) / get_seconds(gs->seconds_for_turn));
        vector4 color = V4(sin((t - 3) * pi * 0.5f),
                           -cos((t + 1) * pi * 0.5f),
                           0, 1);
        ctx->render_square(
                       matrix4::translate_y((float32) (ctx->viewport.height - 10)) *
                       matrix4::scale(ctx->viewport.width * t, 2, 1)
            , color, SHADER_COLOR);
    }

    // Render action buffer
    {
        int x = 200;
        int y = 20;
        for (auto action : gs->action_buffer)
        {
            auto color = action.kind == ENTITY_ACTION2_MOVE ? V4(0.4, 0.4, 0.8, 1) :
                         action.kind == ENTITY_ACTION2_ATTACK ? V4(0.8, 0.4, 0.4, 1) :
                         action.kind == ENTITY_ACTION2_DEFENCE ? V4(0.4, 0.8, 0.4, 1) :
                         V4(0.2, 0.2, 0.2, 1);
            ctx->render_ui(
                           matrix4::translate_x((float32) x) *
                           matrix4::translate_y((float32) y) *
                           matrix4::scale(10, 10, 1)
                , color);

            x += 25;
        }
    }
}

#if DLL_BUILD
#include <context.cpp>
#include <string_id.cpp>
#include <memory_bucket.cpp>
#include <memory/allocator.cpp>
#include <collision.cpp>
#include <image/png.cpp>
#include <crc.cpp>
#include <ecs/entity_manager.cpp>
#endif // DLL_BUILD

#include <ecs/entity_id.cpp>

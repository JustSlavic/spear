#ifndef GAME_SYSTEMS_UPDATE_HPP
#define GAME_SYSTEMS_UPDATE_HPP

#include <base.h>
#include "a_star.hpp"


namespace game {


void find_selection_tile(context *ctx, game_state *gs, input_state *input)
{
    vector3 ray_direction = game::compute_pointer_ray(ctx, gs, input);
    vector3 intersection;
    {
        auto Oxy = make_plane3(0, 0, 1, 0);
        auto line = make_line3(ray_direction, gs->camera.position);
        auto intersection_p = outer(Oxy, line);
        intersection = intersection_p.vector / intersection_p.w;
    }

    gs->intersected = false;
    gs->intersect_t = infinity;
    gs->intersect_x = 0;
    gs->intersect_y = 0;
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto center = V3(x + 1.3f*x, y + 1.3f*y, 0);
            rectangle3 aabb = rectangle3::from_center_radius(center, 1, 1, 1);

            float tmin;
            int intersect_cube = intersect_ray_aabb(gs->camera.position, ray_direction, aabb, &tmin);

            if (intersect_cube && tmin < gs->intersect_t)
            {
                gs->intersected = true;
                gs->intersect_t = tmin;
                gs->intersect_x = x;
                gs->intersect_y = y;
            }
        }
    }
}


void next_turn(context *ctx, game_state *gs, input_state *input)
{
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

        if (hero) for (auto monster_eid : gs->monsters)
        {
            auto *monster = game::get_entity(gs, monster_eid);
            a_star_move moves[25] = {};
            bool32 path_exists = a_star(ctx, gs, monster->x, monster->y, hero->x, hero->y, moves, ARRAY_COUNT(moves), true);
            if (path_exists)
            {
                int dx = 0;
                int dy = 0;

                if (moves[0] == P_ML) dx = -1;
                if (moves[0] == P_MR) dx = 1;
                if (moves[0] == P_MU) dy = 1;
                if (moves[0] == P_MD) dy = -1;

                int x = monster->x + dx;
                int y = monster->y + dy;

                entity_action2 act;
                act.eid = monster->eid;
                act.x0 = monster->x;
                act.y0 = monster->y;
                act.x1 = x;
                act.y1 = y;

                if (x == hero->x && y == hero->y)
                {
                    act.kind = ENTITY_ACTION2_ATTACK;
                    monster->action.kind = ENTITY_ACTION_RIGHT_ARM;
                }
                else
                {
                    act.kind = ENTITY_ACTION2_MOVE;
                    monster->action.kind = ENTITY_ACTION_MOVE;
                }
                monster->action.x = x;
                monster->action.y = y;
                gs->action_buffer.push_back(act);
            }
        }

        gs->turn_no += 1;
        gs->turn_start_time = input->time;
    }
}


} // namespace game

#endif // GAME_SYSTEMS_UPDATE_HPP

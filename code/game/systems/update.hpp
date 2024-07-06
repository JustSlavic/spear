#ifndef GAME_SYSTEMS_UPDATE_HPP
#define GAME_SYSTEMS_UPDATE_HPP

#include <base.h>
#include "a_star.hpp"


namespace game {


entity *get_entity(game_state *gs, ecs::entity_id eid)
{
    entity *result = NULL;
    if (eid != ecs::INVALID_ENTITY_ID)
    {
        result = gs->entities + eid.get_index();
    }
    return result;
}

ecs::entity_id get_active_entity_eid(game_state *gs)
{
    ecs::entity_id result = gs->is_in_battle
        ? gs->battle_queue[gs->turn_no % gs->battle_queue.size()]
        : gs->hero_eid;
    return result;
}

entity *get_active_entity(game_state *gs)
{
    ecs::entity_id eid = get_active_entity_eid(gs);
    entity *result = get_entity(gs, eid);
    return result;
}


void battle_queue_push(game_state *gs, ecs::entity_id eid)
{
    ASSERT(gs->battle_queue.size() < gs->battle_queue.capacity());

    if (gs->battle_queue.size() < gs->battle_queue.capacity())
    {
        gs->battle_queue.push_back(eid);
    }
}

void remove_entity(game_state *gs, entity *e)
{
    if (e->kind == ENTITY_HERO)
    {
        gs->hero_eid = ecs::INVALID_ENTITY_ID;
    }
    if (e->kind == ENTITY_MONSTER)
    {
        gs->monsters.erase_first_unsorted(e->eid);
    }

    gs->set_map_eid(e->x, e->y, ecs::INVALID_ENTITY_ID);
    gs->entity_manager.destroy_entity(e->eid);
    gs->battle_queue.erase_first(e->eid);

    if (gs->selected_entity_eid == e->eid)
    {
        gs->selected_entity_eid = get_active_entity_eid(gs);
    }
}

void on_entity_spawned(game_state *gs, entity *e)
{
    if (gs->is_in_battle)
    {
        gs->battle_queue.push_back(e->eid);
    }
}

ecs::entity_id spawn_entity(game_state *gs, int x, int y, entity **p)
{
    ecs::entity_id eid = ecs::INVALID_ENTITY_ID;
    if (cell_is_empty(gs, x, y))
    {
        eid = gs->entity_manager.create_entity();
        auto *entity = gs->entities + eid.get_index();
        entity->eid = eid;
        entity->x = x;
        entity->y = y;
        gs->set_map_eid(x, y, eid);

        if (p) *p = entity;
    }
    return eid;
}

ecs::entity_id spawn_hero(game_state *gs, int x, int y)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, &e);
    if (e)
    {
        e->kind = ENTITY_HERO;
        e->hp = 3;
        e->max_hp = 5;
        e->invincible = false;
        e->strength = 1;
        e->agility = 1;
    }

    gs->selected_entity_eid = eid;
    gs->hero_eid = eid;
    console::print("hero_id = %d\n", eid.id);

    on_entity_spawned(gs, e);

    return eid;
}

ecs::entity_id spawn_monster(game_state *gs, int x, int y)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, &e);
    if (e)
    {
        e->kind = ENTITY_MONSTER;
        e->hp = 1;
        e->max_hp = 2;
        e->invincible = false;
        e->strength = 1;
        e->agility = 1;
    }

    gs->monsters.push_back(eid);
    console::print("monster_eid = %d\n", eid.id);

    on_entity_spawned(gs, e);

    return eid;
}

ecs::entity_id spawn_stone(game_state *gs, int x, int y)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, &e);
    if (e)
    {
        e->kind = ENTITY_STONE;
        e->invincible = true;
    }

    gs->stones.push_back(eid);
    console::print("stone_eid = %d\n", eid.id);
    return eid;
}


vector3 compute_pointer_ray(context *ctx, game_state *gs, input_state *input)
{
    auto mouse_pos_x =  cvt((float32) input->mouse.x,
        (float32) ctx->viewport.offset_x, (float32) ctx->viewport.offset_x + ctx->viewport.width,
        -1.f, 1.f);
    auto mouse_pos_y = -cvt((float32) input->mouse.y,
        (float32) ctx->viewport.offset_y, (float32) ctx->viewport.offset_y + ctx->viewport.height,
        -1.f, 1.f);

    auto clip_d = ctx->near_clip_dist;
    auto clip_w = ctx->near_clip_width;
    auto clip_h = ctx->near_clip_height;

    auto up = gs->camera.up;
    auto right = cross(gs->camera.forward, gs->camera.up);

    auto clip_c = gs->camera.position + gs->camera.forward * clip_d;
    auto clip_p = clip_c + mouse_pos_x * 0.5f * clip_w * right +
                           mouse_pos_y * 0.5f * clip_h * up;
    vector3 result = normalized(clip_p - gs->camera.position);
    return result;
}

void find_selection_tile(context *ctx, game_state *gs, input_state *input)
{
    vector3 ray_direction = compute_pointer_ray(ctx, gs, input);
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

void move_entity(game_state *gs, entity *e, int x, int y)
{
    if (gs->get_map_eid(x, y) != ecs::INVALID_ENTITY_ID)
        return;

    gs->set_map_eid(e->x, e->y, ecs::INVALID_ENTITY_ID);
    gs->set_map_eid(x, y, e->eid);

    e->x = x;
    e->y = y;
}

void get_entity_movement(game_state *gs, input_state *input, entity *e, int *x, int *y)
{
    int dx = 0;
    int dy = 0;

    if (gs->intersected && get_press_count(input->mouse[MOUSE_LEFT]))
    {
        dx = gs->intersect_x - e->x;
        dy = gs->intersect_y - e->y;
    }
    if (get_press_count(input->keyboard[KB_W])) dy += 1;
    if (get_press_count(input->keyboard[KB_A])) dx -= 1;
    if (get_press_count(input->keyboard[KB_S])) dy -= 1;
    if (get_press_count(input->keyboard[KB_D])) dx += 1;

    *x = e->x + dx;
    *y = e->y + dy;
}

void move_selected_entity(context *ctx, game_state *gs, input_state *input)
{
    entity *selected_entity = game::get_entity(gs, gs->selected_entity_eid);
    if (selected_entity)
    {
        int x = 0, y = 0;
        get_entity_movement(gs, input, selected_entity, &x, &y);
        if (game::entity_can_walk_here(gs, selected_entity, x, y))
        {
            move_entity(gs, selected_entity, x, y);
        }
    }
}


void cmd_start_battle(game_state *gs)
{
    gs->battle_queue.push_back(gs->hero_eid);
    for (auto monster_eid : gs->monsters)
    {
        gs->battle_queue.push_back(monster_eid);
    }
    gs->action_input = null_action();
    gs->selecting_direction_of_action = false;
    gs->turn_no = 0;
    gs->is_in_battle = true;
}

void cmd_end_battle(game_state *gs)
{
    gs->battle_queue.clear();
    gs->is_in_battle = false;
}

void debug_toggle_battle(context *ctx, game_state *gs, input_state *input)
{
    if (get_press_count(input->keyboard[KB_K]))
    {
        if (gs->is_in_battle)
        {
            cmd_end_battle(gs);
        }
        else
        {
            cmd_start_battle(gs);
        }
    }
}


void choose_entity_action(context *ctx, game_state *gs, input_state *input)
{
    if (gs->battle_queue.size() < 2)
    {
        cmd_end_battle(gs);
        return;
    }

    entity *active_entity = get_entity(gs, gs->selected_entity_eid);

    int x, y;
    get_entity_movement(gs, input, active_entity, &x, &y);

    if (get_press_count(input->keyboard[KB_Q]))
    {
        gs->action_input.kind = ENTITY_ACTION_LEFT_ARM;
        TOGGLE(gs->selecting_direction_of_action);
    }
    else if (get_press_count(input->keyboard[KB_E]))
    {
        gs->action_input.kind = ENTITY_ACTION_RIGHT_ARM;
        TOGGLE(gs->selecting_direction_of_action);
    }
    else
    {

    }

    if (gs->is_coords_valid(x, y) &&
        game::cell_is_adjacent_to_entity(active_entity, x, y))
    {
        gs->action_input.x = x;
        gs->action_input.y = y;
        if (!gs->selecting_direction_of_action)
        {
            gs->action_input.kind = ENTITY_ACTION_MOVE;
        }

        if (gs->action_input.kind == ENTITY_ACTION_MOVE)
        {
            if (game::entity_can_walk_here(gs, active_entity, gs->action_input.x, gs->action_input.y))
            {
                active_entity->action = gs->action_input;
                gs->selecting_direction_of_action = false;
            }
        }
        else if (gs->action_input.kind != ENTITY_ACTION_NONE)
        {
            active_entity->action = gs->action_input;
            gs->selecting_direction_of_action = false;
        }
    }
}

void apply_entity_action(game_state *gs, entity *e)
{
    e->state = idle_state();
    auto enemy_eid = gs->get_map_eid(e->action.x, e->action.y);
    if (e->action.kind == ENTITY_ACTION_MOVE)
    {
        console::print("%d at (%d, %d) moves to (%d, %d)\n",
            e->eid.id, e->x, e->y, e->action.x, e->action.y);
        move_entity(gs, e, e->action.x, e->action.y);
    }
    else if (e->action.kind == ENTITY_ACTION_RIGHT_ARM)
    {
        console::print("%d at (%d, %d) attacks %d at (%d, %d)\n",
            e->eid.id, e->x, e->y,
            enemy_eid.id, e->action.x, e->action.y);

        auto eid = gs->get_map_eid(e->action.x, e->action.y);
        if (eid == ecs::INVALID_ENTITY_ID) return;

        auto *attacker = e;
        auto *victim = get_entity(gs, eid);

        if (!((victim->state.kind == ENTITY_STATE_DEFENCE) &&
              (victim->state.x == attacker->x) &&
              (victim->state.y == attacker->y)))
        {
            victim->hp -= 1;
        }
    }
    else if (e->action.kind == ENTITY_ACTION_LEFT_ARM)
    {
        console::print("%d at (%d, %d) defences from %d at (%d, %d)\n",
            e->eid.id, e->x, e->y,
            enemy_eid.id, e->action.x, e->action.y);

        e->state.kind = ENTITY_STATE_DEFENCE;
        e->state.x = e->action.x;
        e->state.y = e->action.y;
    }
}

void next_turn(context *ctx, game_state *gs, input_state *input)
{
    if (gs->battle_queue.size() < 2)
    {
        cmd_end_battle(gs);
        return;
    }

    bool force_new_turn = get_press_count(input->keyboard[KB_SPACE]);
    bool timer_new_turn = gs->turn_timer_enabled &&
                          (input->time >= (gs->turn_start_time + gs->seconds_for_turn));

    if (force_new_turn || timer_new_turn)
    {
        // @attention NEW TURN !!!
        console::print("> new turn \n");

        entity *active_entity = get_active_entity(gs);
        apply_entity_action(gs, active_entity);
        active_entity->action = null_action();

        gs->turn_no += 1;
        gs->selected_entity_eid = get_active_entity_eid(gs);
    }
}

void remove_dead_entities(context *, game_state *gs, input_state *)
{
    auto *hero = get_entity(gs, gs->hero_eid);
    if (hero->hp <= 0)
    {
        remove_entity(gs, hero);
    }
    for (auto monster_eid : gs->monsters)
    {
        auto *monster = get_entity(gs, monster_eid);
        if (monster->hp <= 0)
        {
            remove_entity(gs, monster);
        }
    }
}


} // namespace game

#endif // GAME_SYSTEMS_UPDATE_HPP

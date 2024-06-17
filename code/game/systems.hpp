#ifndef GAME_SYSTEMS_HPP
#define GAME_SYSTEMS_HPP

#include <base.h>
#include <console.hpp>
#include "game.hpp"


namespace game {


void move_camera(game_state *gs, input_state *input)
{
    auto camera_move_direction = V3(0, 0, 0);
    if (get_hold_count(input->keyboard[KB_A])) camera_move_direction -= V3(1, 0, 0);
    if (get_hold_count(input->keyboard[KB_D])) camera_move_direction += V3(1, 0, 0);
    if (get_hold_count(input->keyboard[KB_W])) camera_move_direction += V3(0, 1, 0);
    if (get_hold_count(input->keyboard[KB_S])) camera_move_direction -= V3(0, 1, 0);
    // if (get_hold_count(input->keyboard[KB_R])) camera_move_direction += V3(0, 0, 1);
    // if (get_hold_count(input->keyboard[KB_F])) camera_move_direction -= V3(0, 0, 1);

    if (get_hold_count(input->gamepads[0][GP_DPAD_LEFT])) camera_move_direction -= V3(1, 0, 0);
    if (get_hold_count(input->gamepads[0][GP_DPAD_RIGHT])) camera_move_direction += V3(1, 0, 0);
    if (get_hold_count(input->gamepads[0][GP_DPAD_UP])) camera_move_direction += V3(0, 1, 0);
    if (get_hold_count(input->gamepads[0][GP_DPAD_DOWN])) camera_move_direction -= V3(0, 1, 0);
    if (get_hold_count(input->gamepads[0][GP_LEFT_SHOULDER])) camera_move_direction += V3(0, 0, 1);
    if (get_hold_count(input->gamepads[0][GP_RIGHT_SHOULDER])) camera_move_direction -= V3(0, 0, 1);

    camera_move_direction += V3(input->gamepads[0].left_stick.x, input->gamepads[0].left_stick.y, 0);

    if (input->mouse.scroll != 0)
    {
        float k = 15.f * gs->camera.position.z;
        camera_move_direction += k * input->mouse.scroll * gs->camera.forward;
    }

    gs->camera.position += normalized(camera_move_direction) * gs->camera_speed * input->dt;
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

entity *get_entity(game_state *gs, ecs::entity_id eid)
{
    entity *result = NULL;
    if (eid != ecs::INVALID_ENTITY_ID)
    {
        result = gs->entities + eid.get_index();
    }
    return result;
}

entity_action null_action()
{
    entity_action result = {};
    result.kind = ENTITY_ACTION_NONE;
    return result;
}

entity_state idle_state()
{
    entity_state result = {};
    result.kind = ENTITY_STATE_IDLE;
    return result;
}

entity_action2 get_action2(entity *e)
{
    entity_action2 result;
    result.kind = (e->action.kind == ENTITY_ACTION_MOVE) ? ENTITY_ACTION2_MOVE :
                  (e->action.kind == ENTITY_ACTION_LEFT_ARM) ? ENTITY_ACTION2_DEFENCE :
                  (e->action.kind == ENTITY_ACTION_RIGHT_ARM) ? ENTITY_ACTION2_ATTACK :
                  ENTITY_ACTION2_NONE;
    result.eid = e->eid;
    result.x0 = e->x;
    result.y0 = e->y;
    result.x1 = e->action.x;
    result.y1 = e->action.y;
    return result;
}

bool cell_is_empty(game_state *gs, int x, int y)
{
    if (!gs->is_coords_valid(x, y)) return false;
    return gs->get_map_eid(x, y) == ecs::INVALID_ENTITY_ID;
}

bool cell_is_adjacent_to_entity(entity *e, int x, int y)
{
    return e &&
           ((x == e->x + 1 && y == e->y) ||
            (x == e->x - 1 && y == e->y) ||
            (y == e->y + 1 && x == e->x) ||
            (y == e->y - 1 && x == e->x));
}

bool entity_can_walk_here(game_state *gs, entity *e, int x, int y)
{
    return e && cell_is_empty(gs, x, y) && cell_is_adjacent_to_entity(e, x, y);
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

ecs::entity_id spawn_entity(game_state *gs, int x, int y, entity **p = NULL)
{
    ecs::entity_id eid = ecs::INVALID_ENTITY_ID;
    if (cell_is_empty(gs, x, y))
    {
        eid = gs->entity_manager.create_entity();
        auto *entity = gs->entities + eid.get_index();
        entity->eid = eid;
        entity->x = x;
        entity->y = y;
        entity->action = null_action();
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

void apply_entity_action(game_state *gs, entity *e, entity_action2 action)
{
    if (action.kind == ENTITY_ACTION2_MOVE)
    {
        console::print("%d at (%d, %d) moves to %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
        move_entity(gs, e, action.x1, action.y1);
    }
    else if (action.kind == ENTITY_ACTION2_ATTACK)
    {
        console::print("%d at (%d, %d) attacks %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);

        auto eid = gs->get_map_eid(action.x1, action.y1);
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
    else if (action.kind == ENTITY_ACTION2_DEFENCE)
    {
        console::print("%d at (%d, %d) defences from %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
        e->state.kind = ENTITY_STATE_DEFENCE;
        e->state.x = action.x1;
        e->state.y = action.y1;
    }
}

void apply_actions(game_state *gs)
{
    console::print("-------- turn %d --------\n", gs->turn_no);
    for (auto action : gs->action_buffer)
    {
        auto *e = get_entity(gs, action.eid);
        apply_entity_action(gs, e, action);
        e->action = null_action();
    }
}

void reset_entity_states(game_state *gs)
{
    if (gs->hero_eid != ecs::INVALID_ENTITY_ID)
    {
        auto *hero = get_entity(gs, gs->hero_eid);
        hero->state = idle_state();
    }
    for (auto monster_eid : gs->monsters)
    {
        auto *monster = get_entity(gs, monster_eid);
        monster->state = idle_state();
    }
}

void remove_dead_entity(game_state *gs, entity *e)
{
    if (e && e->hp <= 0)
    {
        if (gs->selected_entity_eid == e->eid)
        {
            gs->selected_entity_eid = ecs::INVALID_ENTITY_ID;
        }
        gs->entity_manager.destroy_entity(e->eid);
        if (e->kind == ENTITY_HERO)
        {
            gs->hero_eid = ecs::INVALID_ENTITY_ID;
        }
        else if (e->kind == ENTITY_MONSTER)
        {
            for (int i = 0; i < gs->monsters.size(); i++)
            {
                if (gs->monsters[i] == e->eid)
                {
                    gs->monsters.erase_not_sorted(i);
                    break;
                }
            }
        }
        gs->set_map_eid(e->x, e->y, ecs::INVALID_ENTITY_ID);
    }
}

void remove_dead_entities(game_state *gs)
{
    remove_dead_entity(gs, get_entity(gs, gs->hero_eid));
    for (auto monster_eid : gs->monsters)
    {
        remove_dead_entity(gs, get_entity(gs, monster_eid));
    }
}


} // namespace game

#endif // GAME_SYSTEMS_HPP

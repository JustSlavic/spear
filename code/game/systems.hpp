#ifndef GAME_SYSTEMS_HPP
#define GAME_SYSTEMS_HPP

#include <base.h>

#include "game.hpp"


namespace game {


void move_camera(game_state *gs, input_state *input)
{
    auto camera_move_direction = V3(0, 0, 0);
    if (get_hold_count(input->keyboard[KB_LEFT])) camera_move_direction -= V3(1, 0, 0);
    if (get_hold_count(input->keyboard[KB_RIGHT])) camera_move_direction += V3(1, 0, 0);
    if (get_hold_count(input->keyboard[KB_UP])) camera_move_direction += V3(0, 1, 0);
    if (get_hold_count(input->keyboard[KB_DOWN])) camera_move_direction -= V3(0, 1, 0);
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
    entity *result = gs->entities + eid.get_index();
    return result;
}

entity_action null_action()
{
    entity_action result = {};
    result.kind = ENTITY_ACTION_NONE;
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

bool cell_is_adjacent_to_entity(entity *hero, int x, int y)
{
    return (x == hero->x + 1 && y == hero->y) ||
           (x == hero->x - 1 && y == hero->y) ||
           (y == hero->y + 1 && x == hero->x) ||
           (y == hero->y - 1 && x == hero->x);
}

bool entity_can_walk_here(game_state *gs, entity *hero, int x, int y)
{
    return cell_is_empty(gs, x, y) && cell_is_adjacent_to_entity(hero, x, y);
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

ecs::entity_id spawn_entity(game_state *gs, int x, int y, entity_kind kind, entity **p = NULL)
{
    ecs::entity_id eid = ecs::INVALID_ENTITY_ID;
    if (cell_is_empty(gs, x, y))
    {
        eid = gs->entity_manager.create_entity();
        auto *entity = gs->entities + eid.get_index();
        entity->eid = eid;
        entity->kind = kind;
        entity->action = null_action();
        entity->x = x;
        entity->y = y;
        gs->set_map_eid(x, y, eid);

        if (p) *p = entity;
    }
    return eid;
}

ecs::entity_id spawn_hero(game_state *gs, int x, int y)
{
    entity *entity = NULL;

    auto eid = spawn_entity(gs, x, y, ENTITY_HERO, &entity);
    if (entity)
    {
        entity->hp = 10;
        entity->strength = 2;
        entity->agility = 2;
    }
 
    gs->selected_entity_eid = eid;
    gs->hero_eid = eid;
    printf("hero_id = %d\n", eid.id);
    return eid;
}

ecs::entity_id spawn_monster(game_state *gs, int x, int y)
{
    auto eid = spawn_entity(gs, x, y, ENTITY_MONSTER);
    gs->monsters[gs->monster_count++] = eid;
    printf("monster_eid = %d\n", eid.id);
    return eid;
}

void apply_entity_action(game_state *gs, entity *e, entity_action2 action)
{
    if (action.kind == ENTITY_ACTION2_MOVE)
    {
        printf("%d at (%d, %d) moves to %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
        move_entity(gs, e, action.x1, action.y1);
    }
    else if (action.kind == ENTITY_ACTION2_ATTACK)
    {
        printf("%d at (%d, %d) attacks %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
    }
    else if (action.kind == ENTITY_ACTION2_DEFENCE)
    {
        printf("%d at (%d, %d) defences from %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
    }
}

void apply_actions(game_state *gs)
{
    printf("-------- turn %d --------\n", gs->turn_no);
    for (auto action : gs->action_buffer)
    {
        auto *e = get_entity(gs, action.eid);
        apply_entity_action(gs, e, action);
        e->action = null_action();
    }
}

// void apply_monster_actions(game_state `


} // namespace game

#endif // GAME_SYSTEMS_HPP

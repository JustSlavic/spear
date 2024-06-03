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


entity_action null_action()
{
    entity_action result = {};
    result.kind = ENTITY_ACTION_NONE;
    return result;
}

bool cell_on_board(int x, int y)
{
    return (-2 <= x && x <= 2) && (-2 <= y && y <= 2);
}

bool cell_is_empty(game_state *gs, int x, int y)
{
    if (!cell_on_board(x, y)) return false;
    return !gs->map_cell_occupied[x][y];
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

ecs::entity_id spawn_entity(game_state *gs, int x, int y, entity_kind kind)
{
    ecs::entity_id eid = ecs::INVALID_ENTITY_ID;
    if (cell_is_empty(gs, x, y))
    {
        eid = gs->entity_manager.create_entity();
        auto *entity = gs->entities + eid.get_index();
        entity->kind = kind;
        entity->action = null_action();
        entity->x = x;
        entity->y = y;
        gs->map_cell_occupied[x][y] = true;
    }
    return eid;
}

ecs::entity_id spawn_hero(game_state *gs, int x, int y)
{
    auto eid = spawn_entity(gs, x, y, ENTITY_HERO);
    gs->hero_id = eid;
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

void apply_entity_action(game_state *gs, entity *e)
{
    if (e->action.kind == ENTITY_ACTION_MOVE)
    {
        gs->map_cell_occupied[e->x][e->y] = false;
        e->x = e->action.x;
        e->y = e->action.y;
        gs->map_cell_occupied[e->x][e->y] = true;
    }
}

void apply_monster_actions(game_state *gs)
{
    for (uint32 i = 0; i < gs->monster_count; i++)
    {
        auto monster_eid = gs->monsters[i];
        entity *monster = gs->entities + monster_eid.get_index();

        apply_entity_action(gs, monster);
    }
}



} // namespace game

#endif // GAME_SYSTEMS_HPP

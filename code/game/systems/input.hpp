#ifndef GAME_SYSTEMS_INPUT_HPP
#define GAME_SYSTEMS_INPUT_HPP

#include <base.h>
#include "entity.hpp"


namespace game {


void game_exit(context *ctx, game_state *gs, input_state *input)
{
    if (get_release_count(gs, input, PlayerAction_ExitGame))
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
}

#if DEBUG
void debug_draw_input(context *ctx, game_state *gs, input_state *input)
{
    if (get_press_count(gs, input, PlayerAction_DebugDraw_Off))
    {
        ctx->debug_draw_off();
    }
    if (get_press_count(gs, input, PlayerAction_DebugDraw_Fps))
    {
        ctx->debug_draw_fps();
    }
}
#endif

void camera_fly_mode(context *ctx, game_state *gs, input_state *input)
{
    if (get_press_count(gs, input, PlayerAction_ToggleFreeCamera))
    {
        TOGGLE(gs->camera_fly_mode);
    }
}

void ghost_view_mode(context *ctx, game_state *gs, input_state *input)
{
    if (get_press_count(input->keyboard[KB_G]))
    {
        gs->world_view = gs->world_view == WORLD_VIEW__NORMAL ? WORLD_VIEW__GHOST :
                         gs->world_view == WORLD_VIEW__GHOST ? WORLD_VIEW__NORMAL : WORLD_VIEW__NORMAL;
    }
}

void camera_movement(game_state *gs, input_state *input)
{
    auto camera_move_direction = V3(0, 0, 0);
    auto camera_rotate_q = quaternion::identity();
    if (get_hold_count(gs, input, PlayerAction_MoveCameraForward)) camera_move_direction += gs->camera.forward;
    if (get_hold_count(gs, input, PlayerAction_MoveCameraBackward)) camera_move_direction -= gs->camera.forward;
    if (get_hold_count(gs, input, PlayerAction_MoveCameraLeft)) camera_move_direction -= cross(gs->camera.forward, gs->camera.up);
    if (get_hold_count(gs, input, PlayerAction_MoveCameraRight)) camera_move_direction += cross(gs->camera.forward, gs->camera.up);

    if (get_hold_count(gs, input, PlayerAction_MoveCameraUp)) camera_move_direction += gs->camera.up;
    if (get_hold_count(gs, input, PlayerAction_MoveCameraDown)) camera_move_direction -= gs->camera.up;

    if (get_hold_count(gs, input, PlayerAction_RotateCameraUp)) camera_rotate_q *= quaternion::rotate(0.01f, cross(gs->camera.forward, gs->camera.up));
    if (get_hold_count(gs, input, PlayerAction_RotateCameraDown)) camera_rotate_q *= quaternion::rotate(-0.01f, cross(gs->camera.forward, gs->camera.up));
    if (get_hold_count(gs, input, PlayerAction_RotateCameraLeft)) camera_rotate_q *= quaternion::rotate(0.01f, gs->camera.up);
    if (get_hold_count(gs, input, PlayerAction_RotateCameraRight)) camera_rotate_q *= quaternion::rotate(-0.01f, gs->camera.up);
    if (get_hold_count(gs, input, PlayerAction_RotateCameraRollLeft)) camera_rotate_q *= quaternion::rotate(-0.01f, gs->camera.forward);
    if (get_hold_count(gs, input, PlayerAction_RotateCameraRollRight)) camera_rotate_q *= quaternion::rotate(0.01f, gs->camera.forward);
    // if (get_hold_count(gs, input, PlayerAction_RotateCameraUp)) camera_rotate_q = gs->camera.up * gs->camera.forward;

    if (get_hold_count(input->gamepads[0][GP_DPAD_LEFT])) camera_move_direction -= V3(1, 0, 0);
    if (get_hold_count(input->gamepads[0][GP_DPAD_RIGHT])) camera_move_direction += V3(1, 0, 0);
    if (get_hold_count(input->gamepads[0][GP_DPAD_UP])) camera_move_direction += V3(0, 1, 0);
    if (get_hold_count(input->gamepads[0][GP_DPAD_DOWN])) camera_move_direction -= V3(0, 1, 0);
    if (get_hold_count(input->gamepads[0][GP_LEFT_SHOULDER])) camera_move_direction += V3(0, 0, 1);
    if (get_hold_count(input->gamepads[0][GP_RIGHT_SHOULDER])) camera_move_direction -= V3(0, 0, 1);

    camera_move_direction += V3(input->gamepads[0].left_stick.x, input->gamepads[0].left_stick.y, 0);

    // if (input->mouse.scroll != 0)
    // {
    //     float k = 15.f * gs->camera.position.z;
    //     camera_move_direction += k * input->mouse.scroll * gs->camera.forward;
    // }

    gs->camera.position += normalized(camera_move_direction) * gs->camera_speed * input->dt;
    gs->camera.forward = apply_unit_quaternion(camera_rotate_q, gs->camera.forward);
    gs->camera.up = apply_unit_quaternion(camera_rotate_q, gs->camera.up);
}

void move_camera(context *ctx, game_state *gs, input_state *input)
{
    if (get_press_count(gs, input, PlayerAction_Follow1)) gs->entity_to_follow = gs->planets[0];
    if (get_press_count(gs, input, PlayerAction_Follow2)) gs->entity_to_follow = gs->planets[1];
    if (get_press_count(gs, input, PlayerAction_Follow3)) gs->entity_to_follow = gs->planets[2];
    if (get_press_count(gs, input, PlayerAction_Follow4)) gs->entity_to_follow = gs->planets[3];
    if (gs->entity_to_follow != ecs::INVALID_ENTITY_ID)
    {
        float32 d_distance = 0.f;
        if (get_hold_count(gs, input, PlayerAction_MoveCameraBackward))
        {
            d_distance += 1.f;
        }
        if (get_hold_count(gs, input, PlayerAction_MoveCameraForward))
        {
            d_distance -= 1.f;
        }
        gs->follow_distance += d_distance * gs->camera_speed * input->dt;
    }
    if (gs->camera_fly_mode)
    {
        camera_movement(gs, input);
    }
}

void heal_hero(context *ctx, game_state *gs, input_state *input)
{
    entity *hero = game::get_entity(gs, gs->hero_eid);
    if (get_press_count(input->keyboard[KB_H]))
    {
        if (hero)
        {
            hero->hp = clamp(hero->hp + 1, 0, hero->max_hp);
        }
    }
}

void spawn_entities(context *ctx, game_state *gs, input_state *input)
{
    if (gs->intersected)
    {
        if (game::cell_is_empty(gs, gs->intersect_x, gs->intersect_y))
        {
            if (get_press_count(gs, input, PlayerAction_SpawnMonster))
            {
                game::spawn_monster(gs, gs->intersect_x, gs->intersect_y);
            }
            else if (get_press_count(gs, input, PlayerAction_SpawnStone))
            {
                game::spawn_stone(gs, gs->intersect_x, gs->intersect_y);
            }
        }
    }
}

void select_entity(context *ctx, game_state *gs, input_state *input)
{
    entity *selected_entity = game::get_entity(gs, gs->selected_entity_eid);
    if (gs->intersected && gs->get_map_eid(gs->intersect_x, gs->intersect_y) != ecs::INVALID_ENTITY_ID &&
        !game::entity_can_walk_here(gs, selected_entity, gs->intersect_x, gs->intersect_y) &&
        !gs->selecting_direction_of_action)
    {
        if (get_press_count(input->mouse[MOUSE_LEFT]))
        {
            gs->selected_entity_eid = gs->get_map_eid(gs->intersect_x, gs->intersect_y);
            selected_entity = game::get_entity(gs, gs->selected_entity_eid);
        }
    }
}


} // namespace game


#endif // GAME_SYSTEMS_INPUT_HPP

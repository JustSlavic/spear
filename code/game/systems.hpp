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


} // namespace game

#endif // GAME_SYSTEMS_HPP

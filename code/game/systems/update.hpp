#ifndef GAME_SYSTEMS_UPDATE_HPP
#define GAME_SYSTEMS_UPDATE_HPP

#include <base.h>
#include "entity.hpp"


namespace game {

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

ECS_SYSTEM(find_selection_tile)
{
    vector3 ray_direction = compute_pointer_ray(ctx, gs, input);
    vector3 intersection;
    {
        auto Oxy = make_plane3(0, 0, 1, 0);
        auto line = make_line3(ray_direction, gs->camera.position);
        auto intersection_p = outer(Oxy, line);
        intersection = intersection_p.vector / intersection_p.w;
    }

    float32 offset_x = (gs->grid_size_x % 2) * 0.5f;
    float32 offset_y = (gs->grid_size_y % 2) * 0.5f;

    console::print("%f %f\n", offset_x, offset_y);

    gs->intersected = true;
    gs->intersect_t = infinity;
    gs->intersect_x = clamp(round(intersection.x), -(gs->grid_size_x - gs->grid_size_x % 2)*0.5f, (gs->grid_size_x - gs->grid_size_x % 2)*0.5f);
    gs->intersect_y = clamp(round(intersection.y), -(gs->grid_size_y - gs->grid_size_y % 2)*0.5f, (gs->grid_size_y - gs->grid_size_y % 2)*0.5f);
}

} // namespace game

#endif // GAME_SYSTEMS_UPDATE_HPP

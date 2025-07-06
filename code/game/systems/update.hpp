#ifndef GAME_SYSTEMS_UPDATE_HPP
#define GAME_SYSTEMS_UPDATE_HPP

#include <base.h>
#include "entity.hpp"
#include "a_star.hpp"


namespace game {


void battle_queue_push(game_state *gs, ecs::entity_id eid)
{
    ASSERT(gs->battle_queue.size() < gs->battle_queue.capacity());

    if (gs->battle_queue.size() < gs->battle_queue.capacity())
    {
        gs->battle_queue.push_back(eid);
    }
}

// @feature1
vector3 compute_pointer_ray(context *ctx, game_state *gs, input_state *input)
{
    auto mouse_pos_x =  cvt((float32) input->keyboard_and_mouse.x,
        (float32) ctx->viewport.offset_x, (float32) ctx->viewport.offset_x + ctx->viewport.width,
        -1.f, 1.f);
    auto mouse_pos_y = -cvt((float32) input->keyboard_and_mouse.y,
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

// @feature1
void find_intersection_with_ground(context *ctx, game_state *gs, input_state *input)
{
    gs->intersected = false;
    gs->intersect_t = infinity;
    gs->intersect_i = 0;
    gs->intersect_j = 0;
    gs->intersect_k = 0;
    gs->intersection = V3(0);

    vector3 ray_direction = compute_pointer_ray(ctx, gs, input);
    for (uint32 k = 0; k < gs->map.dim_z; k++)
    {
        for (uint32 j = 0; j < gs->map.dim_y; j++)
        {
            for (uint32 i = 0; i < gs->map.dim_x; i++)
            {
                if (gs->map.get(i, j, k) == GameMapOccupation_Ground)
                {
                    auto x = (float32) i - gs->map.origin_x;
                    auto y = (float32) j - gs->map.origin_y;
                    auto z = (float32) k - gs->map.origin_z;
                    auto center = V3(x, y, z);
                    auto r = 0.45f; // @todo: pull this from game state
                    rectangle3 aabb = rectangle3::from_center_radius(center, r, r, r);

                    float t;
                    int intersect_cube = intersect_ray_aabb(gs->camera.position, ray_direction, aabb, &t);

                    if (intersect_cube && t < gs->intersect_t)
                    {
                        gs->intersected = true;
                        gs->intersect_t = t;
                        gs->intersect_i = i;
                        gs->intersect_j = j;
                        gs->intersect_k = k;
                        gs->intersection = gs->camera.position + ray_direction * t;
                    }
                }
            }
        }
    }
}

void camera_follow(context *, game_state *gs, input_state *)
{
    auto *e = get_entity(gs, gs->entity_to_follow);
    if (e)
    {
        gs->camera.position = e->position - normalized(gs->camera.forward) * gs->follow_distance;
    }
}

// @feature3 - 06.07.2025
void update_move_animations(context *ctx, game_state *gs, input_state *input)
{
    if (auto e = get_entity(gs, gs->hero_eid))
    {
        if (input->time < e->move_animation_end_time)
        {
            auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
            snprintf((char *) buffer.data, 63,
                "move_animation_t = %f", e->move_animation_t);
            ctx->render_text(
                matrix4::translate(100.f, 280.f, 0.f),
                V4(1), (char const *) buffer.data);
            e->move_animation_t += input->dt;
        }
    }
}


} // namespace game

#endif // GAME_SYSTEMS_UPDATE_HPP

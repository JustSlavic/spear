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
    gs->intersect_tile = make_vector3i(0);
    gs->intersection = V3(0);

    vector3 ray_direction = compute_pointer_ray(ctx, gs, input);
    for (uint32 k = 0; k < gs->map.dim.z; k++)
    {
        for (uint32 j = 0; j < gs->map.dim.y; j++)
        {
            for (uint32 i = 0; i < gs->map.dim.x; i++)
            {
                if (gs->map.get(i, j, k) == GameMapOccupation_Ground)
                {
                    auto center = V3(i, j, k);
                    auto r = 0.45f; // @todo: pull this from game state
                    rectangle3 aabb = rectangle3::from_center_radius(center, r, r, r);

                    float t;
                    int intersect_cube = intersect_ray_aabb(gs->camera.position, ray_direction, aabb, &t);

                    if (intersect_cube && t < gs->intersect_t)
                    {
                        gs->intersected = true;
                        gs->intersect_t = t;
                        gs->intersect_tile = make_vector3i(i, j, k);
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
            vector3 from = to_vector3(e->move_from);
            vector3 to = to_vector3(e->move_to);
            float32 t = cvt(e->move_animation_t, 0.f, e->move_animation_duration, 0.f, 1.f);
            vector3 p = lerp(from, to, t);
            e->position = p;

            {
                auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
                snprintf((char *) buffer.data, 63,
                    "t = %f p = (%5.2f, %5.2f, %5.2f)", e->move_animation_t,
                        p.x, p.y, p.z);
                ctx->render_text(
                    matrix4::translate(100.f, 280.f, 0.f),
                    V4(1), (char const *) buffer.data);
            }

            e->move_animation_t += input->dt;
        }

        {
            auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
            snprintf((char *) buffer.data, 63,
                "e->move_animation_t = %f", e->move_animation_t);
            ctx->render_text(
                matrix4::translate(100.f, 320.f, 0.f),
                V4(1), (char const *) buffer.data);
        }
    }
}


} // namespace game

#endif // GAME_SYSTEMS_UPDATE_HPP

#include <corelibs/base.h>
#include <corelibs/collision.h>


vector3 compute_pointer_ray(context *ctx, game_state *gs, input *input)
{
    float mouse_pos_x =  cvtf(input->keyboard_and_mouse.mouse_x,
        ctx->viewport_offset_x, ctx->viewport_offset_x + ctx->viewport_width,
        -1.f, 1.f);
    float mouse_pos_y = -cvtf(input->keyboard_and_mouse.mouse_y,
        ctx->viewport_offset_y, ctx->viewport_offset_y + ctx->viewport_height,
        -1.f, 1.f);

    float32 clip_d = ctx->near_clip_distance;
    float32 clip_w = ctx->near_clip_width;
    float32 clip_h = ctx->near_clip_height;

    vector3 up = gs->camera.up;
    vector3 right = v3f_cross(gs->camera.forward, gs->camera.up);

    vector3 clip_c = v3f_add(gs->camera.position, v3f_scale(clip_d, gs->camera.forward));
    vector3 clip_p =
        v3f_add(
            clip_c,
            v3f_add(
                v3f_scale(mouse_pos_x * 0.5f * clip_w, right),
                v3f_scale(mouse_pos_y * 0.5f * clip_h, up)));
    vector3 result = v3f_normalize(v3f_sub(clip_p, gs->camera.position));
    return result;
}

void find_intersection_with_ground(context *ctx, game_state *gs, input *input)
{
    gs->intersected = false;
    gs->intersect_t = infinity;
    gs->intersect_tile = v3i(0, 0, 0);
    gs->intersection = v3f(0.f, 0.f, 0.f);

    vector3 ray_direction = compute_pointer_ray(ctx, gs, input);
    UNUSED(ray_direction);
    int i, j, k;
    for (k = 0; k < gs->map.dim.z; k++)
    for (j = 0; j < gs->map.dim.y; j++)
    for (i = 0; i < gs->map.dim.x; i++)
    {
        game_map_cell *cell = game_map_get(&gs->map, i, j, k);
        if (cell->tag == GameMap_Ground)
        {
            vector3 center = v3f(i, j, k);
            float32 factor = 0.45f; // @todo: pull this from game state
            vector3 aabb_min = v3f_sub(center, v3f(factor, factor, factor));
            vector3 aabb_max = v3f_add(center, v3f(factor, factor, factor));

            float t = intersect_ray_aabb(gs->camera.position, ray_direction, aabb_min, aabb_max);
            bool32 intersected = (t < 1000.f);
            if (intersected && t < gs->intersect_t)
            {
                gs->intersected = true;
                gs->intersect_t = t;
                gs->intersect_tile = v3i(i, j, k);
                gs->intersection = v3f_add(gs->camera.position, v3f_scale(t, ray_direction));
            }
        }
        if (cell->tag == GameMap_Entity)
        {
        }
    }
}

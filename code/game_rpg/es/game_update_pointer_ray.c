#include <corelibs/base.h>
#include <corelibs/collision.h>


vector3 compute_pointer_ray(context *ctx, game_state *gs, input *input)
{
    float mouse_pos_x = cvt((float) input->keyboard_and_mouse.window_mouse.x,
        (float) ctx->viewport_offset_x, (float) (ctx->viewport_offset_x + ctx->viewport_width),
        -1.f, 1.f);
    float mouse_pos_y = -cvt((float) input->keyboard_and_mouse.window_mouse.y,
        (float) ctx->viewport_offset_y, (float) (ctx->viewport_offset_y + ctx->viewport_height),
        -1.f, 1.f);

    float32 clip_d = ctx->near_clip_distance;
    float32 clip_w = ctx->near_clip_width;
    float32 clip_h = ctx->near_clip_height;

    vector3 up = gs->camera.up;
    vector3 right = vector3_cross(gs->camera.forward, gs->camera.up);

    vector3 clip_c = vector3_add(gs->camera.position, vector3_scale(clip_d, gs->camera.forward));
    vector3 clip_p =
        vector3_add(
            clip_c,
            vector3_add(
                vector3_scale(mouse_pos_x * 0.5f * clip_w, right),
                vector3_scale(mouse_pos_y * 0.5f * clip_h, up)));
    vector3 result = vector3_normalize(vector3_sub(clip_p, gs->camera.position));
    return result;
}

void find_intersection_with_ground(context *ctx, game_state *gs, input *input)
{
    gs->intersected = false;
    gs->intersect_t = INFINITY;
    gs->intersect_tile = vector3i_create(0, 0, 0);
    gs->intersection = vector3_create(0.f, 0.f, 0.f);

    if (gs->ui.hot)
    {
        return;
    }

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
            vector3 center = vector3_create((float) i, (float) j, (float) k);
            float32 factor = 0.45f; // @todo: pull this from game state
            vector3 aabb_min = vector3_sub(center, vector3_create(factor, factor, factor));
            vector3 aabb_max = vector3_add(center, vector3_create(factor, factor, factor));

            float t = intersect_ray_aabb(gs->camera.position, ray_direction, aabb_min, aabb_max);
            bool32 intersected = (t < 1000.f);
            if (intersected && t < gs->intersect_t)
            {
                gs->intersected = true;
                gs->intersect_t = t;
                gs->intersect_tile = vector3i_create(i, j, k);
                gs->intersection = vector3_add(gs->camera.position, vector3_scale(t, ray_direction));
            }
        }
        if (cell->tag == GameMap_Entity)
        {
        }
    }
}

#include <corelibs/base.h>
#include <engine/game_interface.h>

#include <stdio.h>
#include <math.h>

#include "input/input.c"
#include "update/hero_movement.c"
#include "update/pointer_ray.c"
#include "render/camera.c"
#include "render/map.c"
#include "render/ui.c"
#include "render/debug_pointer_ray.c"

void game_input_stage(context *ctx, game_state *gs, input *input)
{
    game_input_exit(ctx, gs, input);
    game_input_camera_move(ctx, gs, input);
    game_input_hero_move(ctx, gs, input);
}

void game_update_stage(context *ctx, game_state *gs, input *input)
{
    find_intersection_with_ground(ctx, gs, input);
    game_update_hero_move(ctx, gs, input);
    game_update_hero_coordinates(ctx, gs, input);
}

void game_render_stage(context *ctx, game_state *gs, input *input)
{
    game_camera_setup(ctx, gs, input);
    game_render_draw_map(ctx, gs, input);
    game_render_pointer_intersection(ctx, gs, input);
    game_render_ui(ctx, gs, input);
}

void game_on_every_frame(context *ctx, game_state *gs, input *input)
{
    game_input_stage(ctx, gs, input);
    game_update_stage(ctx, gs, input);
    game_render_stage(ctx, gs, input);
}

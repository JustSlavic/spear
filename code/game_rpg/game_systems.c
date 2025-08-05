#include <corelibs/base.h>
#include <engine/game_interface.h>

#include <stdio.h>

#include "update.c"


void game_input_exit(context *ctx, game_state *gs, input *input)
{
    if (input_button_get_release_count(input->keyboard_and_mouse.buttons[Keyboard_Esc]))
    {
        if ((input->time - gs->exit_press_time) < 1.f)
        {
            context_engine_command_push_exit(ctx);
        }
        else
        {
            gs->exit_press_time = input->time;
        }
    }
}

void game_input_camera_move(context *ctx, game_state *gs, input *input)
{
    // if (!gs->is_free_camera) return;

    float32 move_speed = 5.f;
    vector3 right = v3f_cross(gs->camera.forward, gs->camera.up);

    vector3 dp = v3f(0, 0, 0);
    quaternion dq = q4f_identity();

    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_A]))
        dp = v3f_sub(dp, right);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_D]))
        dp = v3f_add(dp, right);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_W]))
        dp = v3f_add(dp, gs->camera.up);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_S]))
        dp = v3f_sub(dp, gs->camera.up);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_F]))
        dp = v3f_add(dp, gs->camera.forward);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_B]))
        dp = v3f_sub(dp, gs->camera.forward);

    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Up]))
        dq = q4f_mul(q4f_rotate(0.01f, right), dq);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Down]))
        dq = q4f_mul(q4f_rotate(-0.01f, right), dq);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Left]))
        dq = q4f_mul(q4f_rotate(0.01f, gs->camera.up), dq);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Right]))
        dq = q4f_mul(q4f_rotate(-0.01f, gs->camera.up), dq);

    dp = v3f_normalize(dp);
    gs->camera.position = v3f_add(v3f_scale(input->dt * move_speed, dp), gs->camera.position);
    gs->camera.forward = q4f_apply_unit_quaternion(dq, gs->camera.forward);
    gs->camera.up = q4f_apply_unit_quaternion(dq, gs->camera.up);
}

void game_input_hero_move(context *ctx, game_state *gs, input *input)
{
    if (gs->intersected)
    if (input_button_get_press_count(input->keyboard_and_mouse.buttons[Mouse_Left]))
    {
        entity *e = get_entity(gs, gs->hero);
        e->move_from = v3i(e->position.x, e->position.y, e->position.z);
        e->move_to = gs->intersect_tile;
        e->move_animation_t = input->time;
        e->move_animation_start_time = input->time;
        e->move_animation_end_time = input->time + e->move_animation_duration;
    }
}

void game_update_hero_move(context *ctx, game_state *gs, input *input)
{
    entity *e = get_entity(gs, gs->hero);
    if (e->move_animation_t > e->move_animation_end_time)
        return;

    float duration = e->move_animation_duration;
    float relative_t = e->move_animation_t - e->move_animation_start_time;
    float normalized_t = relative_t / duration;

    float start_x = (float) e->move_from.x;
    float end_x = (float) e->move_to.x;

    float start_y = (float) e->move_from.y;
    float end_y = (float) e->move_to.y;

    float x = lerpf(start_x, end_x, normalized_t);
    float y = lerpf(start_y, end_y, normalized_t);
    e->position.x = x;
    e->position.y = y;

    e->move_animation_t += input->dt;
}

void game_camera_setup(context *ctx, game_state *gs, input *input)
{
    context_render_command_push_camera(ctx, gs->camera.position, gs->camera.forward, gs->camera.up);
}

void game_render_entity(context *ctx, game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);

    vector4 color = (e->tag == Entity_Hero) ? v4f(0.1, 0.6, 0.3, 1.f)
                  : v4f(0.5, 0.4, 0.2, 1.f);
    context_render_command_push_cube(ctx, RenderCommand_DrawShader_SingleColor,
        e->position,
        v3f(0.45f, 0.45f, 0.45f),
        color);
}

void game_render_draw_map(context *ctx, game_state *gs, input *input)
{
    int i, j, k;
    for (k = 0; k < gs->map.dim.z; k++)
    for (j = 0; j < gs->map.dim.y; j++)
    for (i = 0; i < gs->map.dim.x; i++)
    {
        game_map_cell *cell = game_map_get(&gs->map, i, j, k);
        if (cell->tag == GameMap_Ground)
        {
            float red = 0.2f;
            float green = cvtf((float) i,
                0.f, (float) (gs->map.dim.x - 1),
                0.5f, 0.2f);
            float blue = cvtf((float) i,
                0.f, (float) (gs->map.dim.x - 1),
                0.2f, 0.5f);

            vector4 color = v4f(red, green, blue, 1.f);
            if (i == gs->intersect_tile.x &&
                j == gs->intersect_tile.y &&
                k == gs->intersect_tile.z)
            {
                color.rgb = v3f_scale(1.2f, color.rgb);
            }

            context_render_command_push_cube(ctx, RenderCommand_DrawShader_Ground,
                v3f(i, j, k),
                v3f(0.45f, 0.45f, 0.45f),
                color);
        }
        if (cell->tag == GameMap_Entity)
        {
            game_render_entity(ctx, gs, cell->eid);
        }
    }
}

void game_render_pointer_intersection(context *ctx, game_state *gs, input *input)
{
    if (gs->intersected)
    {
        context_render_command_push_cube(ctx,
            RenderCommand_DrawShader_Ground,
            gs->intersection,
            v3f(0.1f, 0.1f, 0.1f),
            v4f(0.8f, 0.2f, 0.8f, 1.f));
    }
}


void game_on_every_frame(context *ctx, game_state *gs, input *input)
{
    game_input_exit(ctx, gs, input);
    game_input_camera_move(ctx, gs, input);
    game_input_hero_move(ctx, gs, input);
    // Update
    find_intersection_with_ground(ctx, gs, input);
    game_update_hero_move(ctx, gs, input);
    // Render
    game_camera_setup(ctx, gs, input);
    game_render_draw_map(ctx, gs, input);
    game_render_pointer_intersection(ctx, gs, input);
}

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
    if (!gs->is_free_camera) return;

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
    int move_x = 0;
    int move_y = 0;
    if (input_button_get_press_count(input->keyboard_and_mouse.buttons[Keyboard_A]))
        move_x -= 1;
    if (input_button_get_press_count(input->keyboard_and_mouse.buttons[Keyboard_D]))
        move_x += 1;
    if (input_button_get_press_count(input->keyboard_and_mouse.buttons[Keyboard_W]))
        move_y += 1;
    if (input_button_get_press_count(input->keyboard_and_mouse.buttons[Keyboard_S]))
        move_y -= 1;

    if (move_x != 0 || move_y != 0)
    {
        entity *e = get_entity(gs, gs->hero);
        e->move_from = e->tile;
        e->move_to = v3i(e->tile.x + move_x, e->tile.y + move_y, e->tile.z);
        e->move_animation_t = 0.f;
        e->move_animation_start_time = input->time;
        e->move_animation_end_time = input->time + e->move_animation_duration;
    }
}

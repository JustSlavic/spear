void game_input_exit(context *ctx, game_state *gs, spear_input *input)
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

void game_input_camera_move(context *ctx, game_state *gs, spear_input *input)
{
    if (!gs->is_free_camera) return;

    float32 move_speed = 5.f;
    vector3 right = vector3_cross(gs->camera.forward, gs->camera.up);

    vector3 dp = vector3_create(0, 0, 0);
    quaternion dq = quaternion_create_identity();

    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_A]))
        dp = vector3_sub(dp, right);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_D]))
        dp = vector3_add(dp, right);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_W]))
        dp = vector3_add(dp, gs->camera.up);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_S]))
        dp = vector3_sub(dp, gs->camera.up);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_F]))
        dp = vector3_add(dp, gs->camera.forward);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_B]))
        dp = vector3_sub(dp, gs->camera.forward);

    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Up]))
        dq = quaternion_mul(quaternion_rotate(0.01f, right), dq);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Down]))
        dq = quaternion_mul(quaternion_rotate(-0.01f, right), dq);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Left]))
        dq = quaternion_mul(quaternion_rotate(0.01f, gs->camera.up), dq);
    if (input_button_get_hold_count(input->keyboard_and_mouse.buttons[Keyboard_Right]))
        dq = quaternion_mul(quaternion_rotate(-0.01f, gs->camera.up), dq);

    dp = vector3_normalize(dp);
    gs->camera.position = vector3_add(vector3_scale((float32) (input->dt * move_speed), dp), gs->camera.position);
    gs->camera.forward = quaternion_apply_unit(dq, gs->camera.forward);
    gs->camera.up = quaternion_apply_unit(dq, gs->camera.up);
}

void game_input_hero_move(context *ctx, game_state *gs, spear_input *input)
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
        float duration = 0.5f;
        entity *e = get_hero(gs);
        game_move_animation_start(e,
            (float) e->tile.x, (float) e->tile.y, (float) input->time,
            (float) e->tile.x + move_x, (float) e->tile.y + move_y, (float) input->time + duration);
        e->tile.x += move_x;
        e->tile.y += move_y;
    }
}

void game_input_hero_spell(context *ctx, game_state *gs, spear_input *input)
{
    if (gs->spell_id && gs->intersected &&
        input_button_get_press_count(input->keyboard_and_mouse.buttons[Mouse_Left]))
    {
        gs->spell_id = Spell_Invalid;
        printf("CAST SPELL at %d, %d, %d\n",
            gs->intersect_tile.x,
            gs->intersect_tile.y,
            gs->intersect_tile.z);
        entity *hero = get_hero(gs);
        float duration = 0.5f;
        game_entity_create_projectile(gs,
            (float) hero->tile.x, (float) hero->tile.y, (float) hero->tile.z, input->time,
            (float) gs->intersect_tile.x, (float) gs->intersect_tile.y, (float) gs->intersect_tile.z + 1,
            input->time + duration);
    }
}

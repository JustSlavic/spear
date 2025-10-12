void game_update_hero_move(context *ctx, game_state *gs, spear_input *input)
{
    entity *e = get_hero(gs);
    if (input->time < e->move_animation_end_time)
    {
        float t = (input->time - e->move_animation_start_time) /
            (e->move_animation_end_time - e->move_animation_start_time);
        float x = lerp(e->move_animation_from_x, e->move_animation_to_x, t);
        float y = lerp(e->move_animation_from_y, e->move_animation_to_y, t);
        e->position.x = x;
        e->position.y = y;
    }
}

void game_update_projectile_movement(context *ctx, game_state *gs, spear_input *input)
{
    uint i;
    for (i = 0; i < gs->projectiles.count; i++)
    {
        entity_id eid = entity_id_array_get(&gs->projectiles, i);
        entity *e = get_entity(gs, eid);
        if (input->time < e->move_animation_end_time)
        {
            float t = (input->time - e->move_animation_start_time) /
                (e->move_animation_end_time - e->move_animation_start_time);
            float x = lerp(e->move_animation_from_x, e->move_animation_to_x, t);
            float y = lerp(e->move_animation_from_y, e->move_animation_to_y, t);
            e->position.x = x;
            e->position.y = y;
        }
        else
        {
            game_entity_destroy(gs, eid);
        }
    }
}

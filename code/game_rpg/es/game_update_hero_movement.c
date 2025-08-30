

void game_update_hero_move(context *ctx, game_state *gs, input *input)
{
    entity *e = get_hero(gs);
    float t = e->move_animation_t;
    float duration = e->move_animation_duration;
    if (t < duration)
    {
        float normalized_t = t / duration;

        float start_x = (float) e->move_from.x;
        float end_x = (float) e->move_to.x;

        float start_y = (float) e->move_from.y;
        float end_y = (float) e->move_to.y;

        float x = lerp(start_x, end_x, normalized_t);
        float y = lerp(start_y, end_y, normalized_t);
        e->position.x = x;
        e->position.y = y;

        e->move_animation_t += (float32) input->dt;
    }
}

void game_update_hero_coordinates(context *ctx, game_state *gs, input *input)
{
    entity *hero = get_hero(gs);
    hero->tile.x = (int) roundf(hero->position.x);
    hero->tile.y = (int) roundf(hero->position.y);
    hero->tile.z = (int) roundf(hero->position.z);

    // printf("hero.p = %f, %f, %f; hero.tile = %i, %i, %i\n",
    //     hero->position.x, hero->position.y, hero->position.z,
    //     hero->tile.x, hero->tile.y, hero->tile.z);
}

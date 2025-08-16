void game_render_entity(context *ctx, game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);

    vector4 color = (e->tag == Entity_Hero) ? v4f(0.7, 0.7, 0.7, 1.f)
                  : v4f(0.5, 0.4, 0.2, 1.f);
    context_render_command_push_cube(ctx, RenderCommand_DrawShader_SingleColor,
        e->position,
        v3f(0.45f, 0.45f, 0.45f),
        color);
}

void game_render_draw_map(context *ctx, game_state *gs, input *input)
{
    entity *hero = get_entity(gs, gs->hero);

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
            // Color ground that mouse pointer points at
            if (i == gs->intersect_tile.x &&
                j == gs->intersect_tile.y &&
                k == gs->intersect_tile.z)
            {
                color.rgb = v3f_scale(1.2f, color.rgb);
            }
            // Color ground under the hero
            if (i == hero->tile.x &&
                j == hero->tile.y &&
                k == hero->tile.z - 1)
            {
                color.rgb = v3f(0.1f, 0.2f, 0.6f);
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

void game_render_entity(context *ctx, game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);

    vector4 color = (e->tag == Entity_Hero) ? vector4_create(0.7f, 0.7f, 0.7f, 1.f)
                  : vector4_create(0.5f, 0.4f, 0.2f, 1.f);
    context_render_command_push_cube(ctx, RenderCommand_DrawShader_Phong,
        e->position,
        vector3_create(0.45f, 0.45f, 0.45f),
        color);
}

void game_render_draw_map(context *ctx, game_state *gs, spear_input *input)
{
    entity *hero = get_hero(gs);

    int i, j, k;
    for (k = 0; k < gs->map.dim.z; k++)
    for (j = 0; j < gs->map.dim.y; j++)
    for (i = 0; i < gs->map.dim.x; i++)
    {
        game_map_cell *cell = game_map_get(&gs->map, i, j, k);
        if (cell->tag == GameMap_Ground)
        {
            float red = 0.2f;
            float green = cvt((float) i,
                0.f, (float) (gs->map.dim.x - 1),
                0.5f, 0.2f);
            float blue = cvt((float) i,
                0.f, (float) (gs->map.dim.x - 1),
                0.2f, 0.5f);

            vector4 color = vector4_create(red, green, blue, 1.f);
            // Color ground that mouse pointer points at
            if (gs->intersected &&
                i == gs->intersect_tile.x &&
                j == gs->intersect_tile.y &&
                k == gs->intersect_tile.z)
            {
                color.rgb = vector3_scale(1.2f, color.rgb);
            }
            // Color ground under the hero
            if (i == hero->tile.x &&
                j == hero->tile.y &&
                k == hero->tile.z - 1)
            {
                color.rgb = vector3_create(0.1f, 0.2f, 0.6f);
            }

            context_render_command_push_cube(ctx, RenderCommand_DrawShader_Ground,
                vector3_create((float) i, (float) j, (float) k),
                vector3_create(0.45f, 0.45f, 0.45f),
                color);
        }
        if (cell->tag == GameMap_Entity)
        {
            game_render_entity(ctx, gs, cell->eid);
        }
    }
}

void game_render_projectiles(context *ctx, game_state *gs, spear_input *input)
{
    uint i;
    for (i = 0; i < gs->projectiles.count; i++)
    {
        entity_id eid = entity_id_array_get(&gs->projectiles, i);
        entity *e = get_entity(gs, eid);
        render_command cmd =
        {
            .tag = RenderCommand_DrawMesh,
            .mesh_tag = RenderCommand_DrawMesh_Cube,
            .mesh_shader_tag = RenderCommand_DrawShader_Phong,
            .mesh_position = e->position,
            .mesh_scale = vector3_create(0.2f, 0.2f, 0.2f),
            .mesh_color = vector4_create(1.f, 0.f, 0.f, 1.f),
        };
        context_render_command_push(ctx, cmd);
    }
}

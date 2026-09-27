void game_render_pointer_intersection(context *ctx, game_state *gs, spear_input *input)
{
    if (gs->intersected)
    {
        context_render_command_push_cube(ctx,
            RenderCommand_DrawShader_Ground,
            gs->intersection,
            vector3_create(0.1f, 0.1f, 0.1f),
            vector4_create(0.8f, 0.2f, 0.8f, 1.f));
    }

    {
        void *buffer = ALLOCATE_BUFFER_(ctx->temporary_allocator, 64);
        snprintf((char *) buffer, 63,
            "Pointer intersection = (%d, %d, %d)",
            gs->intersect_tile.x,
            gs->intersect_tile.y,
            gs->intersect_tile.z);
        render_command cmd = {
            .tag = RenderCommand_UiText,
            .text = string_view_create_from_cstring(buffer),
            .ui_position = vector2_create(0, 0),
        };
        context_render_command_push(ctx, cmd);
    }
}

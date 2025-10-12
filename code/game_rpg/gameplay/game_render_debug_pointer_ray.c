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
}

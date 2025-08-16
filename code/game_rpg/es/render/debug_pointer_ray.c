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

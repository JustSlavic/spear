void game_camera_setup(context *ctx, game_state *gs, spear_input *input)
{
    context_render_command_push_camera(ctx, gs->camera.position, gs->camera.forward, gs->camera.up);
    if (gs->is_wireframe)
    {
        render_command cmd = { .tag = RenderCommand_Wireframe };
        context_render_command_push(ctx, cmd);
    }
}

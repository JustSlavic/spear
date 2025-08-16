void game_camera_setup(context *ctx, game_state *gs, input *input)
{
    context_render_command_push_camera(ctx, gs->camera.position, gs->camera.forward, gs->camera.up);
}

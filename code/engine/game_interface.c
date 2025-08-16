#include "game_interface.h"


static void context_engine_command_push(context *ctx, engine_command cmd)
{
    ASSERT(ctx->engine_commands_count < ARRAY_COUNT(ctx->engine_commands));
    ctx->engine_commands[ctx->engine_commands_count++] = cmd;
}

static void context_render_command_push(context *ctx, render_command cmd)
{
    ASSERT(ctx->render_commands_count < ARRAY_COUNT(ctx->render_commands));
    ctx->render_commands[ctx->render_commands_count++] = cmd;
}

void context_engine_command_push_exit(context *ctx)
{
    engine_command cmd = { .tag = EngineCommand_Exit };
    context_engine_command_push(ctx, cmd);
}

void context_render_command_push_camera(context *ctx, vector3 position, vector3 forward, vector3 up)
{
    render_command cmd = { .tag = RenderCommand_SetupCamera };
    cmd.camera_position = position;
    cmd.camera_forward = forward;
    cmd.camera_up = up;
    context_render_command_push(ctx, cmd);
}

void context_render_command_push_square(context *ctx, render_command_draw_shader_tag shader_tag, vector3 position, quaternion orientation, float scale_x, float scale_y)
{
    render_command cmd = { .tag = RenderCommand_DrawMesh };
    cmd.mesh_tag = RenderCommand_DrawMesh_Square;
    cmd.mesh_shader_tag = shader_tag;
    cmd.mesh_position = position;
    context_render_command_push(ctx, cmd);
}

void context_render_command_push_cube(context *ctx, render_command_draw_shader_tag shader_tag, vector3 position, vector3 scale, vector4 color)
{
    render_command cmd = { .tag = RenderCommand_DrawMesh };
    cmd.mesh_tag = RenderCommand_DrawMesh_Cube;
    cmd.mesh_shader_tag = shader_tag;
    cmd.mesh_position = position;
    cmd.mesh_scale = scale;
    cmd.mesh_color = color;
    context_render_command_push(ctx, cmd);
}

void context_render_command_push_ui(context *ctx, vector2 p, float width, float height, vector4 color, float frame_width, vector4 frame_color)
{
    render_command cmd = { .tag = RenderCommand_DrawUi };
    cmd.ui_position = p;
    cmd.ui_width = width;
    cmd.ui_height = height;
    cmd.ui_color = color;
    cmd.ui_frame_width = frame_width;
    cmd.ui_frame_color = frame_color;
    context_render_command_push(ctx, cmd);
}

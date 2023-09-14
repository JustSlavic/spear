#include <execution_context.hpp>


void push_execution_command(execution_context *context, execution_command cmd)
{
    ASSERT(context->execution_commands.size() < context->execution_commands.capacity());
    context->execution_commands.push(cmd);
}

void push_render_command(execution_context *context, render_command cmd)
{
    ASSERT(context->render_commands.size() < context->render_commands.capacity());
    context->render_commands.push(cmd);
}

execution_command exit_command()
{
    execution_command result;
    result.type = execution_command::exit;

    return result;
}

void push_setup_camera_command(execution_context *context, render_command::command_setup_camera setup_camera)
{
    render_command cmd;
    cmd.type = render_command::command_type::setup_camera;
    cmd.setup_camera = setup_camera;

    push_render_command(context, cmd);
}

void push_draw_background_command(execution_context *context, render_command::command_draw_background draw_bg)
{
    render_command cmd;
    cmd.type = render_command::command_type::draw_background;
    cmd.draw_background = draw_bg;

    push_render_command(context, cmd);
}

void push_draw_mesh_with_color_command(execution_context *context, render_command::command_draw_mesh_with_color draw_mesh)
{
    render_command cmd;
    cmd.type = render_command::command_type::draw_mesh_with_color;
    cmd.draw_mesh_with_color = draw_mesh;

    push_render_command(context, cmd);
}

void push_draw_mesh_with_texture_command(execution_context *context, render_command::command_draw_mesh_with_texture draw_mesh)
{
    render_command cmd;
    cmd.type = render_command::command_type::draw_mesh_with_texture;
    cmd.draw_mesh_with_texture = draw_mesh;

    push_render_command(context, cmd);
}

void push_draw_ui_texture_command(execution_context *context, render_command::command_draw_ui_texture cmd_)
{
    render_command cmd;
    cmd.type = render_command::command_type::draw_ui_texture;
    cmd.draw_ui_texture = cmd_;

    push_render_command(context, cmd);
}

void push_draw_screen_frame(execution_context *context, render_command::command_draw_screen_frame draw_frame)
{
    render_command cmd;
    cmd.type = render_command::command_type::draw_screen_frame;
    cmd.draw_screen_frame = draw_frame;

    push_render_command(context, cmd);
}

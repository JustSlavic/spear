#include <gamelibs/ui.h>


void game_render_ui(context *ctx, game_state *gs, input *input)
{
    int i;
    for (i = 0; i < gs->ui_visibles.count; i++)
    {
        entity *e = get_entity(gs, gs->ui_visibles.data[i]);
        render_command cmd =
        {
            .tag = RenderCommand_DrawUi,
            .ui_mesh_tag = RenderCommand_DrawMesh_Square,
            .ui_shader_tag = RenderCommand_DrawShader_SingleColor,
            .ui_tm = e->ui.tm_to_root,
            .ui_width = e->ui.width,
            .ui_height = e->ui.height,
            .ui_color = e->ui.hovered ? v4f(0, 1, 0, 1) : e->ui.color,
        };
        context_render_command_push(ctx, cmd);
    }
    for (i = 0; i < gs->ui_hoverables.count; i++)
    {
        entity *e = get_entity(gs, gs->ui_visibles.data[i]);

        vector2 min = e->ui.hover_area_min;
        vector2 max = e->ui.hover_area_max;

        // printf("%d: min = %5.2f, %5.2f; max = %5.2f, %5.2f\n",
        //     gs->ui_visibles.data[i],
        //     min.x, min.y, max.x, max.y);
        vector2 size = v2f_sub(max, min);
        vector2 center = v2f_scale(0.5f, v2f_add(min, max));

        render_command cmd =
        {
            .tag = RenderCommand_DrawUi,
            .ui_mesh_tag = RenderCommand_DrawMesh_UiFrame,
            .ui_shader_tag = RenderCommand_DrawShader_SingleColor,
            .ui_tm = e->ui.tm_to_root,
            .ui_width = size.x,
            .ui_height = size.y,
            .ui_color = v4f(0.9, 0.9, 0.9, 1),
            .ui_frame_width = 2,
            .ui_offset = center,
        };
        context_render_command_push(ctx, cmd);
    }
}

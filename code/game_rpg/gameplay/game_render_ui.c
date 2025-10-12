#include <gamelibs/ui.h>


void game_render_ui(context *ctx, game_state *gs, spear_input *input)
{
    uint i;
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
            .ui_color = e->ui.hovered ? vector4_create(0, 1, 0, 1) : e->ui.color,
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
        vector2 size = vector2_sub(max, min);
        vector2 center = vector2_scale(0.5f, vector2_add(min, max));

        render_command cmd =
        {
            .tag = RenderCommand_DrawUi,
            .ui_mesh_tag = RenderCommand_DrawMesh_UiFrame,
            .ui_shader_tag = RenderCommand_DrawShader_SingleColor,
            .ui_tm = e->ui.tm_to_root,
            .ui_width = size.x,
            .ui_height = size.y,
            .ui_color = vector4_create(0.9f, 0.9f, 0.9f, 1.f),
            .ui_frame_width = 2,
            .ui_offset = center,
        };
        context_render_command_push(ctx, cmd);
    }
}

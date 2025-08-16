#include <gamelibs/ui.h>


void game_render_ui(context *ctx, game_state *gs, input *input)
{
    int i;
    for (i = 0; i < gs->ui_element_count; i++)
    {
        entity *e = get_entity(gs, gs->ui_elements[i]);
        if (e)
        {
            if (ui_element_flag_test(&e->ui, UiBehaviour_Visible))
            {
                context_render_command_push_ui(ctx,
                    e->ui.position,
                    e->ui.scale.x * e->ui.width,
                    e->ui.scale.y * e->ui.height,
                    v4f(1, 1, 1, 1),
                    10.f,
                    v4f(1, 0, 0, 1));

                context_render_command_push_ui(ctx,
                    v2f(e->ui.position.x + 106.f, e->ui.position.y),
                    e->ui.scale.x * e->ui.width,
                    e->ui.scale.y * e->ui.height,
                    v4f(1, 1, 1, 1),
                    -10.f,
                    v4f(1, 0, 0, 1));
            }
        }
    }
}

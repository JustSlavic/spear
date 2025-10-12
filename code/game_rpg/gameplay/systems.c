#include <corelibs/base.h>
#include <engine/game_interface.h>

#include <stdio.h>
#include <math.h>

#include "game_input.c"
#include "game_update_hero_movement.c"
#include "game_update_pointer_ray.c"
#include "game_update_ui.c"
#include "game_render_camera.c"
#include "game_render_map.c"
#include "game_render_ui.c"
#include "game_render_debug_pointer_ray.c"


void game_input_stage(context *ctx, game_state *gs, spear_input *input)
{
    game_input_exit(ctx, gs, input);
    game_input_camera_move(ctx, gs, input);
    game_input_hero_move(ctx, gs, input);
    game_input_hero_spell(ctx, gs, input);
}

void game_update_stage(context *ctx, game_state *gs, spear_input *input)
{
    find_intersection_with_ground(ctx, gs, input);
    game_update_hero_move(ctx, gs, input);
    game_update_projectile_movement(ctx, gs, input);
    game_update_ui(ctx, gs, input);
}

void game_render_stage(context *ctx, game_state *gs, spear_input *input)
{
    game_camera_setup(ctx, gs, input);
    game_render_draw_map(ctx, gs, input);
    game_render_projectiles(ctx, gs, input);
    game_render_pointer_intersection(ctx, gs, input);
    game_render_ui(ctx, gs, input);
}

void game_reset_spell_target(game_state *gs)
{
    gs->spell_id = Spell_Invalid;
    printf("spell id := %d\n", gs->spell_id);
}

void game_choose_spell_target(game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);
    if (e->ui_id == EntityUiId_SpellFireball)
    {
        gs->spell_id = Spell_Fireball;
        printf("spell id := %d\n", gs->spell_id);
    }
}

void game_process_event(game_state *gs, event event)
{
    switch (event.tag)
    {
    case Event_UiEnter:
        printf("UI: Enter eid=%d\n", event.eid);
    break;
    case Event_UiLeave:
        printf("UI: Leave eid=%d\n", event.eid);
    break;
    case Event_UiPress:
        printf("UI: Press eid=%d\n", event.eid);
        game_choose_spell_target(gs, event.eid);
    break;
    case Event_UiRelease:
        printf("UI: Release eid=%d\n", event.eid);
    break;

    case Event_Invalid:
        ASSERT_MSG(false, "Unknown event (type=%d)", event.tag);
        return;
    }
}

void game_process_events(game_state *gs)
{
    uint i;
    for (i = 0; i < gs->event_count; i++)
    {
        game_process_event(gs, gs->events[i]);
    }
    gs->event_count = 0;
}

void game_on_every_frame(context *ctx, game_state *gs, spear_input *input)
{
    game_input_stage(ctx, gs, input);
    game_update_stage(ctx, gs, input);
    game_render_stage(ctx, gs, input);

    game_process_events(gs);
}

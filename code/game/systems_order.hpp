#ifndef GAME_SYSTEMS_ORDER_HPP
#define GAME_SYSTEMS_ORDER_HPP

#include "systems/input.hpp"
#include "systems/update.hpp"
#include "systems/render.hpp"


namespace game {


void process_input(context *ctx, game_state *gs, input_state *input)
{
    game_exit(ctx, gs, input);
    camera_fly_mode(ctx, gs, input);
    ghost_view_mode(ctx, gs, input);
    heal_hero(ctx, gs, input);
    spawn_entities(ctx, gs, input);
    move_camera(ctx, gs, input);
    select_entity(ctx, gs, input);
    debug_toggle_battle(ctx, gs, input);
}

void update_stage(context *ctx, game_state *gs, input_state *input)
{
    enter_battle_on_enemies_present(ctx, gs, input);
    find_selection_tile(ctx, gs, input);
    if (gs->is_in_battle)
    {
        entity_action(ctx, gs, input);
    }
    else
    {
        move_selected_entity(ctx, gs, input);
    }
    remove_dead_entities(ctx, gs, input);
    next_turn(ctx, gs, input);
}

void render_stage(context *ctx, game_state *gs, input_state *input)
{
    render_ground(ctx, gs, input);
    render_character_page(ctx, gs, input);
    render_hero(ctx, gs, input);
    render_monsters(ctx, gs, input);
    render_stones(ctx, gs, input);
    render_timer(ctx, gs, input);
    if (gs->is_in_battle)
    {
        render_battle_queue(ctx, gs, input);
    }
    // render_dialogue(ctx, gs, input);
    // render_field(ctx, gs, input);
}

void on_every_frame(context *ctx, game_state *gs, input_state *input)
{
    process_input(ctx, gs, input);
    update_stage(ctx, gs, input);
    render_stage(ctx, gs, input);
}


} // namespace game


#endif // GAME_SYSTEMS_ORDER_HPP

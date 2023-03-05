#include <game.hpp>
#include <game_state.hpp>


//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    game_state *gs = (game_state *) game_memory.memory;
    gs->entity_count = 1; // @note: let zero-indexed entity be 'null entity' representing lack of entity
}

//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{
    game_state *gs = (game_state *) game_memory.memory;

    // push_execution_command(context, {execution_context::command::exit});
}

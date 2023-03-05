#include <game.hpp>


//
// Arguments:
// - memory_block memory;
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    auto p = (uint32_t *)game_memory.memory;
    p[0] = 1;
    p[1] = 2;
    p[3] = 42;
}

//
// Arguments:
// - memory_block memory;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{

}

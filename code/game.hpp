#ifndef GAME_HPP
#define GAME_HPP

#include <base.hpp>
#include <memory.hpp>


#define INITIALIZE_MEMORY_FUNCTION(NAME) void NAME(memory_block game_memory)
typedef INITIALIZE_MEMORY_FUNCTION(initialize_memory_t);

#define UPDATE_AND_RENDER_FUNCTION(NAME) void NAME(memory_block game_memory)
typedef UPDATE_AND_RENDER_FUNCTION(update_and_render_t);

extern "C"
{
DLL_EXPORT INITIALIZE_MEMORY_FUNCTION(initialize_memory);
DLL_EXPORT UPDATE_AND_RENDER_FUNCTION(update_and_render);
}


#endif // GAME_HPP

#ifndef GAME_HPP
#define GAME_HPP

#include <base.hpp>
#include <input.hpp>
#include <memory/memory.hpp>
#include <execution_context.hpp>


#define INITIALIZE_MEMORY_FUNCTION(NAME) void NAME(execution_context *context, memory_block game_memory)
typedef INITIALIZE_MEMORY_FUNCTION(initialize_memory_t);

#define UPDATE_AND_RENDER_FUNCTION(NAME) void NAME(execution_context *context, memory_block game_memory, input_devices *input, float32 dt)
typedef UPDATE_AND_RENDER_FUNCTION(update_and_render_t);

extern "C"
{
DLL_EXPORT INITIALIZE_MEMORY_FUNCTION(initialize_memory);
DLL_EXPORT UPDATE_AND_RENDER_FUNCTION(update_and_render);
}


#endif // GAME_HPP

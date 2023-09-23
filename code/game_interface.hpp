#ifndef GAME_HPP
#define GAME_HPP

#include <base.h>
#include <memory.h>

#include <input.hpp>
#include <execution_context.hpp>


#define INITIALIZE_MEMORY_FUNCTION_T(NAME) void NAME(execution_context *context, memory_block game_memory)
typedef INITIALIZE_MEMORY_FUNCTION_T(initialize_memory_t);
#define INITIALIZE_MEMORY_FUNCTION(...) INITIALIZE_MEMORY_FUNCTION_T(initialize_memory)

#define UPDATE_AND_RENDER_FUNCTION_T(NAME) void NAME(execution_context *context, memory_block game_memory, input_state *input)
typedef UPDATE_AND_RENDER_FUNCTION_T(update_and_render_t);
#define UPDATE_AND_RENDER_FUNCTION(...) UPDATE_AND_RENDER_FUNCTION_T(update_and_render)


extern "C"
{
DLL_EXPORT INITIALIZE_MEMORY_FUNCTION();
DLL_EXPORT UPDATE_AND_RENDER_FUNCTION();
}


#endif // GAME_HPP

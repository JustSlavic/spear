#ifndef GAME_HPP
#define GAME_HPP

#include <base.hpp>
#include <input.hpp>
#include <memory/memory.hpp>
#include <gfx/renderer.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>


struct execution_command
{
    enum command_type
    {
        exit,
    };

    command_type type;
};


struct execution_context
{
    execution_command execution_command_queue[32];
    usize execution_command_queue_size;
    usize next_execution_command_index;

    gfx::render_command render_command_queue[1024];
    usize render_command_queue_size;
};


INLINE void push_execution_command(execution_context *context, execution_command cmd)
{
    ASSERT(context->execution_command_queue_size < ARRAY_COUNT(context->execution_command_queue));
    context->execution_command_queue[context->execution_command_queue_size] = cmd;
    context->execution_command_queue_size += 1;
}

INLINE execution_command pop_execution_command(execution_context *context)
{
    ASSERT(context->execution_command_queue_size > 0);
    auto result = context->execution_command_queue[context->next_execution_command_index];
    context->next_execution_command_index += 1;
    context->execution_command_queue_size -= 1;
    return result;
}

INLINE void push_render_command(execution_context *context, gfx::render_command cmd)
{
    ASSERT(context->render_command_queue_size < ARRAY_COUNT(context->render_command_queue));
    context->render_command_queue[context->render_command_queue_size] = cmd;
    context->render_command_queue_size += 1;
}


#define INITIALIZE_MEMORY_FUNCTION(NAME) void NAME(execution_context *context, memory_block game_memory)
typedef INITIALIZE_MEMORY_FUNCTION(initialize_memory_t);

#define UPDATE_AND_RENDER_FUNCTION(NAME) void NAME(execution_context *context, memory_block game_memory, input_devices *input)
typedef UPDATE_AND_RENDER_FUNCTION(update_and_render_t);

extern "C"
{
DLL_EXPORT INITIALIZE_MEMORY_FUNCTION(initialize_memory);
DLL_EXPORT UPDATE_AND_RENDER_FUNCTION(update_and_render);
}


#endif // GAME_HPP

#ifndef GAME_HPP
#define GAME_HPP

#include <base.hpp>
#include <memory/memory.hpp>


struct execution_context
{
    struct command
    {
        enum command_type
        {
            exit,
        };

        command_type type;
    };

    command command_queue[32];
    usize next_command_index;
    usize command_queue_size;
};


INLINE void push_execution_command(execution_context *context, execution_context::command cmd)
{
    ASSERT(context->command_queue_size < ARRAY_COUNT(context->command_queue));
    context->command_queue[context->command_queue_size] = cmd;
    context->command_queue_size += 1;
}

INLINE execution_context::command pop_execution_command(execution_context *context)
{
    ASSERT(context->command_queue_size > 0);
    auto result = context->command_queue[context->next_command_index];
    context->next_command_index += 1;
    context->command_queue_size -= 1;
    return result;
}


#define INITIALIZE_MEMORY_FUNCTION(NAME) void NAME(execution_context *context, memory_block game_memory)
typedef INITIALIZE_MEMORY_FUNCTION(initialize_memory_t);

#define UPDATE_AND_RENDER_FUNCTION(NAME) void NAME(execution_context *context, memory_block game_memory)
typedef UPDATE_AND_RENDER_FUNCTION(update_and_render_t);

extern "C"
{
DLL_EXPORT INITIALIZE_MEMORY_FUNCTION(initialize_memory);
DLL_EXPORT UPDATE_AND_RENDER_FUNCTION(update_and_render);
}


#endif // GAME_HPP

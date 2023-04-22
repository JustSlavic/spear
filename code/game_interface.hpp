#ifndef GAME_HPP
#define GAME_HPP

#include <base.hpp>

#include <input.hpp>
#include <memory/memory.hpp>
#include <array.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>
#include <resource_system.hpp>


struct execution_command
{
    enum command_type
    {
        exit,

        create_mesh_resource,
        load_mesh_resource,

        create_texture_resource,
        load_texture_resource,

        create_shader_resource,
        load_shader_resource,
    };

    command_type type;

    union
    {
        struct
        {
            float32 *vbo;
        };
    };
};


INLINE execution_command exit_command()
{
    execution_command result;
    result.type = execution_command::exit;

    return result;
}

INLINE execution_command create_mesh_resource_command(float32 *vbo)
{
    execution_command result;
    result.type = execution_command::create_mesh_resource;
    result.vbo = vbo;

    return result;
}


// @todo: move this definition to the game code side of the border
enum debug_time_measure_slot
{
    DEBUG_TIME_SLOT_update_and_render,
    DEBUG_TIME_SLOT_get_world_chunk_slot,
    DEBUG_TIME_SLOT_push_entity_in_world_chunk_slot,
    DEBUG_TIME_SLOT_remove_entity_from_world_chunk_slot,
    DEBUG_TIME_SLOT_put_entity_in_chunk,
    DEBUG_TIME_SLOT_move_entity_between_chunks,
    // ------------------
    DEBUG_TIME_SLOT_COUNT
};

struct debug_time_measurement
{
    uint64 cycle_count;
    uint64 hit_count;
};

FORCE_INLINE void add_measurement(debug_time_measurement *measurement, uint64 cycles)
{
    measurement->cycle_count += cycles;
    measurement->hit_count += 1;
}

struct execution_context
{
    array<execution_command> execution_commands;

    gfx::render_command *render_command_queue;
    usize render_command_queue_capacity;
    usize render_command_queue_size;

    memory::allocator temporary_allocator;
    memory::allocator renderer_allocator;
    rs::resource_storage resource_storage;
    string_id_storage strid_storage;

#if DEBUG
    debug_time_measurement debug_measurements[DEBUG_TIME_SLOT_COUNT];
#endif
};


INLINE void push_execution_command(execution_context *context, execution_command cmd)
{
    ASSERT(context->execution_commands.size < context->execution_commands.capacity);
    context->execution_commands.push_back(cmd);
}


INLINE void push_render_command(execution_context *context, gfx::render_command cmd)
{
    ASSERT(context->render_command_queue_size < context->render_command_queue_capacity);
    context->render_command_queue[context->render_command_queue_size] = cmd;
    context->render_command_queue_size += 1;
}

INLINE void push_setup_camera_command(execution_context *context, gfx::render_command::command_setup_camera setup_camera)
{
    gfx::render_command cmd;
    cmd.type = gfx::render_command::command_type::setup_camera;
    cmd.setup_camera = setup_camera;

    push_render_command(context, cmd);
}

INLINE void push_draw_background_command(execution_context *context, gfx::render_command::command_draw_background draw_bg)
{
    gfx::render_command cmd;
    cmd.type = gfx::render_command::command_type::draw_background;
    cmd.draw_background = draw_bg;

    push_render_command(context, cmd);
}

INLINE void push_draw_mesh_with_color_command(execution_context *context, gfx::render_command::command_draw_mesh_with_color draw_mesh)
{
    gfx::render_command cmd;
    cmd.type = gfx::render_command::command_type::draw_mesh_with_color;
    cmd.draw_mesh_with_color = draw_mesh;

    push_render_command(context, cmd);
}


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

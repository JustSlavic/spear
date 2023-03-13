#ifndef GAME_HPP
#define GAME_HPP

#include <base.hpp>
#include <input.hpp>
#include <memory/memory.hpp>
#include <gfx/renderer.hpp> // @todo: abstract it away, replace with renderer queue, but here should not be a renderer
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


struct execution_context
{
    execution_command execution_command_queue[32];
    usize execution_command_queue_size;
    usize next_execution_command_index;

    gfx::render_command render_command_queue[1024];
    usize render_command_queue_size;

    memory::allocator temporary_allocator;
    memory::allocator renderer_allocator;
    rs::resource_storage resource_storage;
    string_id_storage strid_storage;
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

INLINE void push_draw_mesh_1_command(execution_context *context, gfx::render_command::command_draw_mesh_1 draw_mesh_1)
{
    gfx::render_command cmd;
    cmd.type = gfx::render_command::command_type::draw_mesh_1;
    cmd.draw_mesh_1 = draw_mesh_1;

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

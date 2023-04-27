#ifndef EXECUTION_CONTEXT_HPP
#define EXECUTION_CONTEXT_HPP

#include <base.hpp>
#include <array.hpp>
#include <math/matrix4.hpp>
#include <math/rectangle2.hpp>
#include <rs/resource_system.hpp>


#if DEBUG
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
#endif // DEBUG

struct execution_command
{
    enum command_type
    {
        exit,
    };

    command_type type;
};

struct render_command
{
    enum class command_type
    {
        setup_projection_matrix,
        setup_camera,
        draw_background,
        draw_mesh_1,
        draw_mesh_with_color,
    };
    struct command_setup_projection_matrix
    {
        math::matrix4 projection;
    };
    struct command_setup_camera
    {
        math::vector3 camera_position;
        math::vector3 look_at_position;
        math::vector3 camera_up_direction;
    };
    struct command_draw_background
    {
        rs::resource_token mesh;
        rs::resource_token shader;
        math::vector4 color;
    };
    struct command_draw_rectangle
    {
        math::rectangle2 rect;
        math::matrix4 model;
    };
    struct command_draw_mesh_1
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        math::matrix4 model;
    };
    struct command_draw_mesh_with_color
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        math::matrix4 model;
        math::vector4 color;
    };

    command_type type;
    union
    {
        command_setup_projection_matrix setup_projection_matrix;
        command_setup_camera setup_camera;
        command_draw_background draw_background;
        command_draw_rectangle draw_rectangle;
        command_draw_mesh_1 draw_mesh_1;
        command_draw_mesh_with_color draw_mesh_with_color;
    };
};

struct execution_context
{
    array<execution_command> execution_commands;
    array<render_command> render_commands;

    memory::allocator temporary_allocator;
    memory::allocator renderer_allocator;
    rs::resource_storage resource_storage;
    string_id_storage strid_storage;

#if DEBUG
    debug_time_measurement debug_measurements[DEBUG_TIME_SLOT_COUNT];
#endif
};


void push_execution_command(execution_context *context, execution_command cmd);
void push_render_command(execution_context *context, render_command cmd);

execution_command exit_command();
void push_setup_camera_command(execution_context *context, render_command::command_setup_camera setup_camera);
void push_draw_background_command(execution_context *context, render_command::command_draw_background draw_bg);
void push_draw_mesh_with_color_command(execution_context *context, render_command::command_draw_mesh_with_color draw_mesh);


#endif // EXECUTION_CONTEXT_HPP

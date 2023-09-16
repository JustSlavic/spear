#ifndef EXECUTION_CONTEXT_HPP
#define EXECUTION_CONTEXT_HPP

#include <base.h>
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

enum debug_loop_state
{
    DEBUG_LOOP_IDLE,
    DEBUG_LOOP_RECORDING,
    DEBUG_LOOP_REPLAYING,
};
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
        draw_mesh_with_color,
        draw_mesh_with_texture,
        draw_screen_frame,
        draw_ui,
        draw_ui_texture,
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
    struct command_draw_mesh_with_color
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        math::matrix4 model;
        math::vector4 color;
    };
    struct command_draw_mesh_with_texture
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        rs::resource_token texture_token;
        math::matrix4 model;
    };
    struct command_draw_screen_frame
    {
        math::matrix4 model;
        math::matrix4 view;
        math::matrix4 projection;
        math::vector4 color;
    };
    struct command_draw_ui
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        math::matrix4 model;
        math::matrix4 view;
        math::matrix4 projection;
        math::vector4 color;
    };
    struct command_draw_ui_texture
    {
        rs::resource_token mesh_token;
        rs::resource_token shader_token;
        rs::resource_token texture_token;
        math::matrix4 model;
        math::matrix4 view;
        math::matrix4 projection;
    };

    command_type type;
    union
    {
        command_setup_projection_matrix setup_projection_matrix;
        command_setup_camera setup_camera;
        command_draw_background draw_background;
        command_draw_rectangle draw_rectangle;
        command_draw_mesh_with_color draw_mesh_with_color;
        command_draw_mesh_with_texture draw_mesh_with_texture;
        command_draw_screen_frame draw_screen_frame;
        command_draw_ui draw_ui;
        command_draw_ui_texture draw_ui_texture;
    };
};

struct execution_context
{
    array<execution_command> execution_commands;
    array<render_command> render_commands;

    memory_allocator temporary_allocator;
    memory_allocator renderer_allocator;
    rs::resource_storage resource_storage;
    string_id_storage *strid_storage;

    uint32 screen_width;
    uint32 screen_height;
    
    uint32 window_width;
    uint32 window_height;

    uint32 letterbox_width;
    uint32 letterbox_height;

    memory_block (*debug_load_file)(memory_allocator, char const *);

#if DEBUG
    debug_time_measurement debug_measurements[DEBUG_TIME_SLOT_COUNT];
#endif // DEBUG
};


void push_execution_command(execution_context *context, execution_command cmd);
void push_render_command(execution_context *context, render_command cmd);

execution_command exit_command();
void push_setup_camera_command(execution_context *context, render_command::command_setup_camera setup_camera);
void push_draw_background_command(execution_context *context, render_command::command_draw_background draw_bg);
void push_draw_mesh_with_color_command(execution_context *context, render_command::command_draw_mesh_with_color draw_mesh);
void push_draw_mesh_with_texture_command(execution_context *context, render_command::command_draw_mesh_with_texture draw_mesh);
void push_draw_screen_frame(execution_context *context, render_command::command_draw_screen_frame);


#endif // EXECUTION_CONTEXT_HPP

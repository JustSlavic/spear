#include <game.hpp>
#include <game_state.hpp>

#include <math/rectangle2.hpp>
#include <math/matrix4.hpp>


//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    game_state *gs = (game_state *) game_memory.memory;
    gs->entity_count = 1; // @note: let zero-indexed entity be 'null entity' representing lack of entity
}

//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{
    game_state *gs = (game_state *) game_memory.memory;
    // push_execution_command(context, {execution_context::command::exit});

    gfx::render_command setup_camera_command;
    setup_camera_command.type = gfx::render_command::setup_camera;
    setup_camera_command.camera_position = math::make_vector3(0, 0, -3);
    setup_camera_command.look_at_position = math::make_vector3(0, 0, 0);
    setup_camera_command.camera_up_direction = math::make_vector3(0, 1, 0);

    push_render_command(context, setup_camera_command);

    gfx::render_command render_rectangle;
    render_rectangle.type = gfx::render_command::draw_rectangle;
    render_rectangle.rect = math::rectangle2::from_center_size(math::make_vector2(0, 0), 1, 1);
    render_rectangle.model = math::matrix4::identity();

    push_render_command(context, render_rectangle);
}

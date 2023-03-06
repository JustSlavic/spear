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
    using namespace math;

    game_state *gs = (game_state *) game_memory.memory;
    gs->entity_count = 1; // @note: let zero-indexed entity be 'null entity' representing lack of entity

    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
        {
            auto *entity = push_entity(gs);
            entity->position = make_vector2(0.1f * x, 0.1f * y);
            entity->velocity = make_vector2(0, 0);
            entity->bounding_box = rectangle2::from_center_size(make_vector2(0, 0), 0.05f, 0.05f);
        }
}

//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{
    using namespace math;

    game_state *gs = (game_state *) game_memory.memory;
    // push_execution_command(context, {execution_context::command::exit});

    gfx::render_command setup_camera_command;
    setup_camera_command.type = gfx::render_command::setup_camera;
    setup_camera_command.camera_position = make_vector3(0, 0, -3);
    setup_camera_command.look_at_position = make_vector3(0, 0, 0);
    setup_camera_command.camera_up_direction = make_vector3(0, 1, 0);

    push_render_command(context, setup_camera_command);

    for (int entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *entity = gs->entities + entity_index;

        rectangle2 aabb = entity->bounding_box;
        aabb.min += entity->position;
        aabb.max += entity->position;

        gfx::render_command render_rectangle;
        render_rectangle.type = gfx::render_command::draw_rectangle;
        render_rectangle.rect = aabb;
        render_rectangle.model = matrix4::identity();

        push_render_command(context, render_rectangle);
    }
}

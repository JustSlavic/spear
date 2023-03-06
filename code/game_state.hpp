#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.hpp>
#include <math/vector2.hpp>


struct entity
{
    math::vector2 position;
    math::vector2 velocity;
    math::rectangle2 bounding_box;
};


struct game_state
{
    math::vector3 camera_position;

    entity entities[1024];
    usize  entity_count;
};


INLINE entity *push_entity(game_state *gs)
{
    ASSERT(gs->entity_count < ARRAY_COUNT(gs->entities));
    auto result = gs->entities + gs->entity_count++;
    return result;
}


#endif // GAME_STATE_HPP

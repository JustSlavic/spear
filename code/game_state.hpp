#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.hpp>
#include <resource_system.hpp>
#include <math/vector2.hpp>


struct entity
{
    math::vector2 position;
    math::vector2 velocity;
    float32 width, height;
    float32 rotation;
    float32 rotational_velocity;
};


struct game_state
{
    math::vector3 camera_position;
    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;

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

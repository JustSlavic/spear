#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.hpp>
#include <resource_system.hpp>
#include <math/vector2.hpp>


enum entity_type
{
    ENTITY_INVALID = 0,
    ENTITY_CIRCLE,
    ENTITY_RECTANGLE,
};

struct entity
{
    entity_type type;

    math::vector2 position;
    math::vector2 velocity;
    union
    {
        float32 radius;
        float32 width;
    };
    float32 height;
    float32 mass;
    float32 rotation;
    float32 rotational_velocity;

    math::rectangle2 aabb;
};


struct game_state
{
    math::vector3 camera_position;
    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;
    rs::resource_token circle_shader;

    entity entities[1024];
    usize  entity_count;
};


INLINE entity *push_entity(game_state *gs)
{
    ASSERT(gs->entity_count < ARRAY_COUNT(gs->entities));
    auto result = gs->entities + gs->entity_count++;
    return result;
}

INLINE entity *get_entity(game_state *gs, uint32 index)
{
    ASSERT(index < gs->entity_count);
    auto result = gs->entities + index;
    return result;
}


#endif // GAME_STATE_HPP

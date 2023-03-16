#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.hpp>
#include <game_state.hpp>


bool32 find_collision_point(game_state *gs, uint32 collide_entity_index, math::vector2 *collision_point)
{
    bool32 result = false;
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        if (entity_index == collide_entity_index) continue;

        auto *collide_entity = gs->entities + collide_entity_index;
        auto *test_entity = gs->entities + entity_index;

        if (math::is_intersecting(collide_entity->aabb, test_entity->aabb))
        {
            result = true;
            break;
        }
    }
    return result;
}


#endif // COLLISION_HPP

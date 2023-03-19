#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.hpp>
#include <game_state.hpp>


struct collision_data
{
    math::vector2 point;
    uint32 entity_index;
};


bool32 find_collision_point(game_state *gs, uint32 collide_entity_index, collision_data *collision)
{
    auto *collide_entity = gs->entities + collide_entity_index;

    bool32 result = false;
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        if (entity_index == collide_entity_index) continue;

        auto *test_entity = gs->entities + entity_index;

        if (math::is_intersecting(collide_entity->aabb, test_entity->aabb))
        {
            if (collide_entity->type == ENTITY_CIRCLE && test_entity->type == ENTITY_CIRCLE)
            {
                auto d_sq = math::length_squared(collide_entity->position - test_entity->position);
                auto r_sq = math::square(collide_entity->radius + test_entity->radius);
                if (d_sq < r_sq)
                {
                    result = true;
                    // Step back from test entity on its radius
                    collision->entity_index = entity_index;
                    collision->point = test_entity->position - test_entity->width * normalized(test_entity->position - collide_entity->position);
                    break;
                }
            }
        }
    }
    return result;
}


#endif // COLLISION_HPP

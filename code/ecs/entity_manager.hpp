#ifndef ECS_ENTITY_MANAGER_HPP
#define ECS_ENTITY_MANAGER_HPP

#include <base.h>
#include "entity.hpp"
#include "event.hpp"

/*
    Entity ID is divided into two parts:
    +------------+--------------------+
    | generation | index in the array |
    +------------+--------------------+

    - Generation is bumped whenever the entity is deleted
*/


namespace ecs {

#define ECS_MAX_ENTITIES_POWER_OF_2 2 // 2^15 = 32768
#define ECS_MAX_ENTITIES (1 << ECS_MAX_ENTITIES_POWER_OF_2)

struct entity_manager
{
    uint16 generations[ECS_MAX_ENTITIES];
    uint16 empty_slots[ECS_MAX_ENTITIES];
    uint64 p_read;
    uint64 p_write;
    // ring_buffer<event> event_queue;

    static entity_manager create();

    entity_id create_entity();
    void destroy_entity(entity_id);
    bool is_entity_exists(entity_id);
};


} // namespace ecs


#endif // ECS_ENTITY_MANAGER_HPP

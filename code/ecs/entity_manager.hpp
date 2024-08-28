#ifndef ECS_ENTITY_MANAGER_HPP
#define ECS_ENTITY_MANAGER_HPP

#include <base.h>
#include <memory/allocator.hpp>

#include "entity_id.hpp"
#include "event.hpp"
#include "archetype.hpp"

/*
    Entity ID is divided into two parts:
    +------------+--------------------+
    | generation | index in the array |
    +------------+--------------------+

    - Generation is bumped whenever the entity is deleted
*/


namespace ecs {


struct entity_description
{
    entity_gen_t generation;
};


struct entity_manager
{
    array<entity_description> descriptions;
    array<entity_idx_t> empty_slots;
    uint64 p_read;
    uint64 p_write;

    static entity_manager initialize(memory_allocator a);

    entity_id create_entity();
    void destroy_entity(entity_id);
    bool is_entity_exists(entity_id);

    template <typename EventType>
    void send_event(EventType &&evt);

    template <typename EventType>
    void send_event_immediate(EventType &&evt);
};


template <typename EventType>
void entity_manager::send_event(EventType &&evt)
{
    // @todo: push back the event into a queue
}

template <typename EventType>
void entity_manager::send_event_immediate(EventType &&evt)
{
    // @todo: apply event immediate without pushing into a queue
}


} // namespace ecs


#endif // ECS_ENTITY_MANAGER_HPP

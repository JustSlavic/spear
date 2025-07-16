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

typedef void process_event_cb_t(uint32 event_type, void *event_memory, uint32 event_size);

struct entity_description
{
    entity_gen_t generation;
    archetype_idx_t archetype_index;
    entity_idx_t index_in_archetype;
};

struct entity_manager
{
    memory_allocator allocator;

    array<entity_description> descriptions;
    array<entity_idx_t> empty_slots;
    uint64 p_read;
    uint64 p_write;
    array<archetype> archetypes;

    static entity_manager initialize(memory_allocator a);

    entity_id create_entity(archetype_idx_t archetype_index = 0);
    void destroy_entity(entity_id);
    bool is_entity_exists(entity_id);

    template <typename EventType>
    void send_event(EventType &&evt);

    template <typename EventType>
    void send_event_immediate(EventType &&evt);
};


archetype_idx_t make_archetype(entity_manager *em, uint32 entity_size);

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

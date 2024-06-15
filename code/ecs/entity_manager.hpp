#ifndef ECS_ENTITY_MANAGER_HPP
#define ECS_ENTITY_MANAGER_HPP

#include <base.h>
#include "entity_id.hpp"
#include "event.hpp"

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

};


struct entity_manager
{
    // THIS IS TOO BIG !!!!
    entity_gen_t generations[ECS_MAX_ENTITIES];
    entity_idx_t empty_slots[ECS_MAX_ENTITIES];
    uint64 p_read;
    uint64 p_write;

    event_registry events;

    static entity_manager create();

    entity_id create_entity();
    void destroy_entity(entity_id);
    bool is_entity_exists(entity_id);

    template <uint32 EventTypeHash, typename Fn>
    void register_system(Fn *fn);

    template <typename EventType>
    void send_event(EventType &&evt);

    template <typename EventType>
    void send_event_immediate(EventType &&evt);
};


template <uint32 EventTypeHash, typename Fn>
void entity_manager::register_system(Fn *fn)
{
    event_id_t id = events.find_event<EventTypeHash>();
    if (id == INVALID_EVENT_ID)
        id = events.register_event<EventTypeHash>();

    // @todo: ???
}

template <typename EventType>
void entity_manager::send_event(EventType &&evt)
{
    event_id_t id = events.find_event<EventType>();
    if (id == INVALID_EVENT_ID) return;

    // @todo: push back the event into a queue
}

template <typename EventType>
void entity_manager::send_event_immediate(EventType &&evt)
{
    event_id_t id = events.find_event<EventType>();
    if (id == INVALID_EVENT_ID) return;

    // @todo: apply event immediate without pushing into a queue
}


} // namespace ecs


#endif // ECS_ENTITY_MANAGER_HPP

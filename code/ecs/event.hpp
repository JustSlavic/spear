#ifndef ECS_EVENT_HPP
#define ECS_EVENT_HPP

#include <base.h>


namespace ecs {


typedef uint32 event_id_t;
extern event_id_t INVALID_EVENT_ID;

struct event
{
    // should here be list of systems that should be called when this event fires?
};


struct event_registry
{
    template <typename EventType>
    event_id_t register_event();

    template <typename EventType>
    event_id_t find_event();
};


template <typename EventType>
event_id_t event_registry::register_event()
{
    return INVALID_EVENT_ID;
}


template <typename EventType>
event_id_t event_registry::find_event()
{
    return INVALID_EVENT_ID;
}


} // namespace ecs


#endif // ECS_EVENT_HPP

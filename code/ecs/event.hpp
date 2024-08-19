#ifndef ECS_EVENT_HPP
#define ECS_EVENT_HPP

#include <base.h>


namespace ecs {


typedef uint32 event_id_t;
extern event_id_t INVALID_EVENT_ID;

struct event
{

};

struct event_input_stage
{
    void *input;
};

struct event_update_stage
{
    float32 dt;
};

struct event_render_stage
{
    void *context;
};



} // namespace ecs


#endif // ECS_EVENT_HPP

#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>
#include <memory/allocator.hpp>

#include <ecs/entity_manager.hpp>

#include <game/camera.hpp>


enum entity_kind
{
    ENTITY_NONE
};

struct entity
{
    ecs::entity_id eid;
};

struct input_action
{
    uint32 press;
    uint32 hold;
    uint32 release;
    uint32 double_click;

    timepoint last_press_time;
    timepoint last_release_time;
};


struct game_state
{
    memory_allocator allocator;
    ecs::entity_manager entity_manager;
    entity entities[ECS_MAX_ENTITIES];

    game::camera camera;
    float32 camera_speed;

    timepoint exit_press_time;

    bool32 intersected;
    float32 intersect_t;
    float32 intersect_x;
    float32 intersect_y;

    int grid_size_x;
    int grid_size_y;
};


#endif // GAME_STUB_HPP

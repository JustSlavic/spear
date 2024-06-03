#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>
#include <memory/allocator.hpp>

#include <ecs/entity_manager.hpp>

#include "rs/resource_system.hpp"

#include <game/camera.hpp>


enum entity_action_kind
{
    ENTITY_ACTION_NONE,
    ENTITY_ACTION_MOVE,
    ENTITY_ACTION_LEFT_ARM,
    ENTITY_ACTION_RIGHT_ARM,
};

struct entity_action
{
    entity_action_kind kind;

    int x;
    int y;
};


enum entity_kind
{
    ENTITY_NONE,
    ENTITY_PLAYER,
    ENTITY_HERO,
    ENTITY_MONSTER,
};

struct entity
{
    entity_kind kind;
    entity_action action;

    int x;
    int y;
};


struct game_state
{
    memory_allocator allocator;

    bool map_cell_occupied[5][5];

    entity entities[ECS_MAX_ENTITIES];
    ecs::entity_id monsters[ECS_MAX_ENTITIES];
    uint32 monster_count;

    ecs::entity_manager entity_manager;

    ecs::entity_id hero_id;

    int turn_no;
    duration seconds_for_turn;
    timepoint turn_start_time;

    entity_action action_input;
    bool selecting_direction_of_action;

    game::camera camera;
    float32 camera_speed;

    rs::token rect_mesh;
    rs::token cube_mesh;
    rs::token rect_mesh_uv;
    rs::token text_buffers;

    rs::token shader_single_color;
    rs::token shader_draw_texture;
    rs::token shader_draw_text;
    rs::token shader_ground;

    rs::token font_texture;

    timepoint exit_press_time;
};


#endif // GAME_STUB_HPP

#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>
#include <memory/allocator.hpp>

#include <ecs/entity_manager.hpp>

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

enum entity_state_kind
{
    ENTITY_STATE_IDLE,
    ENTITY_STATE_DEFENCE,
};

struct entity_state
{
    entity_state_kind kind;
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
    ecs::entity_id eid;

    entity_kind kind;
    entity_action action;
    entity_state state;

    int x;
    int y;

    int hp;
    
    int strength;
    int agility;
};

enum entity_action2_kind
{
    ENTITY_ACTION2_NONE,
    ENTITY_ACTION2_MOVE,
    ENTITY_ACTION2_ATTACK,
    ENTITY_ACTION2_DEFENCE,
};

struct entity_action2
{
    entity_action2_kind kind;

    ecs::entity_id eid;

    int x0;
    int y0;

    int x1;
    int y1;
};


struct game_state
{
    memory_allocator allocator;

    entity entities[ECS_MAX_ENTITIES];
    static_array<ecs::entity_id, ECS_MAX_ENTITIES> monsters;

    ecs::entity_manager entity_manager;

    ecs::entity_id hero_eid;
    ecs::entity_id selected_entity_eid;

    static_array<entity_action2, 32> action_buffer;

    bool turn_timer_enabled;
    int turn_no;
    duration seconds_for_turn;
    timepoint turn_start_time;

    entity_action action_input;
    bool selecting_direction_of_action;

    bool spawn_hero_next_turn;

    game::camera camera;
    float32 camera_speed;

    rs::token shader_single_color;
    rs::token shader_draw_texture;
    rs::token shader_draw_text;
    rs::token shader_ground;

    rs::token font_texture;

    timepoint exit_press_time;

    bool is_coords_valid(int x, int y);
    ecs::entity_id get_map_eid(int x, int y);
    void set_map_eid(int x, int y, ecs::entity_id eid);

    ecs::entity_id map[5][5];
};


#endif // GAME_STUB_HPP

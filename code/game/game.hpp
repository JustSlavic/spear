#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>

#include <math/vector4.hpp>
#include <memory/allocator.hpp>
#include <ecs/entity_manager.hpp>

#include <game/camera.hpp>


// #define printf

enum world_view_state
{
    WORLD_VIEW__NORMAL,
    WORLD_VIEW__GHOST,
};

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
    ENTITY_STONE,
};

struct entity
{
    ecs::entity_id eid;

    entity_kind kind;
    entity_action action;
    entity_state state;

    bool invincible;

    int x;
    int y;

    int hp;
    int max_hp;
    
    int strength;
    int agility;
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

// void act_on_press(input_action *action, button_state *button);
// void act_on_release(input_action *action, button_state *button);
// void act_on_hold(input_action *action, button_state *button);
// void act_on_double_click(input_action *action, button_state *button, duration interval_between_presses);

struct player_input_actions
{
    input_action action_exit_game;
    input_action move_west;
    input_action move_east;
    input_action move_north;
    input_action move_south;
    input_action select_left_arm;
    input_action select_right_arm;
    input_action finish_turn;
};


struct game_field
{
    int32 width;
    int32 height;
    array<ecs::entity_id> map;

    bool is_coords_valid(int x, int y);
    ecs::entity_id get_eid(int x, int y);
    void set_eid(int x, int y, ecs::entity_id eid);
};


struct game_state
{
    memory_allocator allocator;

    entity entities[ECS_MAX_ENTITIES];
    static_array<ecs::entity_id, ECS_MAX_ENTITIES> monsters;
    static_array<ecs::entity_id, 25> stones;

    static_array<ecs::entity_id, 32> battle_queue;
    uint32 battle_queue_current_slot;

    ecs::entity_manager entity_manager;

    ecs::entity_id hero_eid;
    ecs::entity_id selected_entity_eid;

    world_view_state world_view;

    bool turn_timer_enabled;
    int turn_no;
    duration seconds_for_turn;
    timepoint turn_start_time;

    entity_action action_input;
    bool selecting_direction_of_action;

    bool spawn_hero_next_turn;

    bool is_in_battle;

    game::camera camera;
    float32 camera_speed;

    bool camera_fly_mode;
    timepoint exit_press_time;

    vector4 move_color;
    vector4 defence_color;
    vector4 attack_color;

    bool32 intersected;
    float32 intersect_t;
    int intersect_x;
    int intersect_y;

    duration double_click_interval;

    float selected_entity_height = 0.8f;
    float regular_entity_height = 0.3f;

    bool is_coords_valid(int x, int y);
    ecs::entity_id get_map_eid(int x, int y);
    void set_map_eid(int x, int y, ecs::entity_id eid);

    ecs::entity_id map[5][5];

    float32 field_render__gap;
    game_field field;
};


#endif // GAME_STUB_HPP

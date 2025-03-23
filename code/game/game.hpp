#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>

#include <input.hpp>
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

    Entity_Planet,
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

    // Planet stuff
    vector3 position;
    vector3 velocity;
    float32 radius;
    float32 mass;
};

enum input_action_bits
{
    InputAction_Press = 0x1,
    InputAction_Release = 0x2,
    InputAction_Hold = 0x4,
    InputAction_DoubleClick = 0x8,
};

enum
{
    PlayerAction_None,
    PlayerAction_ExitGame,

    PlayerAction_ToggleFreeCamera,
    PlayerAction_MoveCameraForward,
    PlayerAction_MoveCameraBackward,
    PlayerAction_MoveCameraLeft,
    PlayerAction_MoveCameraRight,
    PlayerAction_MoveCameraUp,
    PlayerAction_MoveCameraDown,
    PlayerAction_RotateCameraUp,
    PlayerAction_RotateCameraDown,
    PlayerAction_RotateCameraLeft,
    PlayerAction_RotateCameraRight,

    PlayerAction_SpawnMonster,
    PlayerAction_SpawnStone,
};

struct action_set
{
    uint32 buttons[32];
};

struct game_state;

uint32 get_press_count(game_state *gs, input_state *input, uint32 action_id);
uint32 get_release_count(game_state *gs, input_state *input, uint32 action_id);
uint32 get_hold_count(game_state *gs, input_state *input, uint32 action_id);

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
    static_array<ecs::entity_id, 25> planets;

    static_array<ecs::entity_id, 32> battle_queue;
    uint32 battle_queue_current_slot;

    action_set player_actions;

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

#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>

#include <input.hpp>
#include <math.hpp>
#include <phys/world.hpp>
    
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

    // RPG stuff
    int x;
    int y;
    int z;

    int hp;
    int max_hp;
    
    int strength;
    int agility;

    // Planet stuff
    vector3 position;
    float32 radius;
    vector3 color;
    quaternion orientation;

    phys::handle phys_world_handle;
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

#if DEBUG
    PlayerAction_DebugDraw_Off,
    PlayerAction_DebugDraw_Fps,
#endif

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
    PlayerAction_RotateCameraRollLeft,
    PlayerAction_RotateCameraRollRight,

    PlayerAction_FollowPlanetNext,
    PlayerAction_FollowPlanetPrev,
    PlayerAction_FollowPlanetStop,

    PlayerAction_SpawnMonster,
    PlayerAction_SpawnStone,

    PlayerAction_Count
};

struct action_set
{
    uint32 buttons[PlayerAction_Count];
};

struct game_state;

uint32 get_press_count(game_state *gs, input_state *input, uint32 action_id);
uint32 get_release_count(game_state *gs, input_state *input, uint32 action_id);
uint32 get_hold_count(game_state *gs, input_state *input, uint32 action_id);

enum
{
    GameMapOccupation_Empty,
    GameMapOccupation_Ground,
    GameMapOccupation_Entity,
};

struct game_map
{
    uint32 dim_x, dim_y, dim_z;
    uint32 origin_x, origin_y, origin_z;
    array<uint32> data;

    uint32 get(int i, int j, int k)
    {
        return data[k * dim_x * dim_y + j * dim_x + i];
    }

    void set(int i, int j, int k, uint32 v)
    {
        data[k * dim_x * dim_y + j * dim_x + i] = v;
    }
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
    ecs::entity_id hero_eid;
    static_array<ecs::entity_id, ECS_MAX_ENTITIES> monsters;
    static_array<ecs::entity_id, 25> stones;
    static_array<ecs::entity_id, 100> planets;

    phys::world phys_world;

    static_array<ecs::entity_id, 32> battle_queue;
    uint32 battle_queue_current_slot;

    action_set player_actions;

    ecs::entity_manager entity_manager;

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
    // @feature0002 - 2025.07.05
    float32 camera_speed;
    vector3 camera__default_position;
    vector3 camera__default_direction;
    vector3 camera__default_up;

    ecs::entity_id entity_to_follow;
    int32 planet_follow_index;
    float32 follow_distance;
    float32 min_follow_distance;

    bool camera_fly_mode;
    timepoint exit_press_time;

    vector4 move_color;
    vector4 defence_color;
    vector4 attack_color;

    // @feature0001 - 2025.07.05
    bool32 intersected;
    float32 intersect_t;
    int intersect_i;
    int intersect_j;
    int intersect_k;
    vector3 intersection;

    // @todo move this into input library
    duration double_click_interval;

    float selected_entity_height = 0.8f;
    float regular_entity_height = 0.3f;

    bool is_coords_valid(int x, int y);
    ecs::entity_id get_map_eid(int x, int y);
    void set_map_eid(int x, int y, ecs::entity_id eid);

    ecs::entity_id map[5][5];

    float32 field_render__gap;
    game_field field;

    game_map map2;
};


#endif // GAME_STUB_HPP

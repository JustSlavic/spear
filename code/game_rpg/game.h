#ifndef GAME_H
#define GAME_H

#include <corelibs/base.h>
#include <corelibs/math.h>
#include <corelibs/memory/allocator.h>
#include <gamelibs/camera.h>
#include <engine/entity_manager.h>

typedef enum
{
    Entity_None = 0,

    Entity_Hero,
    Entity_Monster,
} entity_tag;

typedef struct entity
{
    uint32 tag;

    vector3i tile;
    vector3 position;

    float32 move_animation_t;
    float32 move_animation_start_time;
    float32 move_animation_end_time;
    float32 move_animation_duration;
    vector3i move_from;
    vector3i move_to;
} entity;

enum
{
    GameMap_Empty = 0,

    GameMap_Ground,
    GameMap_Entity,
};

typedef struct
{
    uint32 tag;
    entity_id eid;
} game_map_cell;

typedef struct game_map
{
    vector3i dim;
    game_map_cell *data;
} game_map;

game_map_cell *game_map_get(game_map *map, int i, int j, int k);
bool32 game_map_set_ground(game_map *map, int i, int j, int k);
bool32 game_map_set_entity(game_map *map, entity_id eid, int i, int j, int k);

typedef struct game_state
{
    memory_allocator game_allocator;

    entity entities[MAX_ENTITIES];
    entity_manager em;

    entity_id hero;

    bool is_free_camera;
    vector3 camera_default_position;
    vector3 camera_default_forward;
    vector3 camera_default_up;
    camera camera;
    game_map map;

    bool32   intersected;
    float32  intersect_t;
    vector3i intersect_tile;
    vector3  intersection;

    float64 exit_press_time;
} game_state;

entity *get_entity(game_state *gs, entity_id eid);

#endif // GAME_H

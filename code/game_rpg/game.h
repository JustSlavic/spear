#ifndef GAME_H
#define GAME_H

#include <corelibs/base.h>
#include <corelibs/math.h>
#include <corelibs/memory/allocator.h>
#include <gamelibs/camera.h>
#include <gamelibs/entity_manager.h>
#include <gamelibs/ui.h>

typedef enum
{
    Entity_None = 0,

    Entity_Hero,
    Entity_Monster,
    Entity_UiElement,
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

    ui_element ui;
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

typedef struct
{
    entity_id *data;
    uint32 count;
    uint32 capacity;
} entity_id_array;

#define entity_id_array_allocate(A, N) ((entity_id_array) { .data = ALLOCATE_ARRAY(A, entity_id, N), .count = 0, .capacity = N })
void entity_id_array_push(entity_id_array *array, entity_id e);

typedef struct game_state
{
    memory_allocator game_allocator;

    entity *entities;

    entity_manager em;
    ui ui;

    entity_id_array heroes;
    entity_id_array monsters;
    entity_id_array ui_elements;
    entity_id_array ui_visibles;
    entity_id_array ui_hoverables;
    entity_id_array ui_clickables;

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

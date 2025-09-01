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
    Entity_Invalid = 0,

    Entity_Hero,
    Entity_Monster,
    Entity_Projectile,
    Entity_UiElement,

    Entity_ArchetypeCount,
} entity_tag;

typedef enum
{
    Spell_Invalid = 0,
    Spell_Fireball,
} spell_id;

typedef enum
{
    EntityUiId_Invalid = 0,
    EntityUiId_SpellFireball,
} entity_ui_id;

typedef struct entity
{
    uint32 tag;

    vector3i tile;
    vector3 position;

    float32 move_animation_start_time;
    float32 move_animation_end_time;
    float32 move_animation_from_x;
    float32 move_animation_from_y;
    float32 move_animation_to_x;
    float32 move_animation_to_y;

    ui_element ui;
    uint32 ui_id;
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
entity_id entity_id_array_get(entity_id_array *array, int index);

typedef enum
{
    Event_Invalid = 0,

    Event_UiEnter,
    Event_UiLeave,
    Event_UiPress,
    Event_UiRelease,
} event_tag;

typedef struct
{
    event_tag tag;
    entity_id eid; // What entity this event is sent to
} event;

typedef struct game_state
{
    memory_allocator game_allocator;

    event *events;
    uint32 event_count;
    uint32 event_capacity;

    entity *entities;
    entity_manager em;
    ui ui;

    entity_id_array heroes;
    entity_id_array monsters;
    entity_id_array projectiles;
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
    int map_ground_level;

    bool32   intersected;
    float32  intersect_t;
    vector3i intersect_tile;
    vector3  intersection;

    float64 exit_press_time;

    uint32 spell_id;
} game_state;

entity *get_entity(game_state *gs, entity_id eid);

#endif // GAME_H

#include "game.h"

#include <engine/game_interface.h>

#include <stdio.h>


void push_event(game_state *gs, event event)
{
    ASSERT(gs->event_count < gs->event_capacity);
    if (gs->event_count < gs->event_capacity)
    {
        gs->events[gs->event_count++] = event;
    }
}

game_map_cell *game_map_get(game_map *map, int i, int j, int k)
{
    ASSERT(0 <= i && i < map->dim.x);
    ASSERT(0 <= j && j < map->dim.y);
    ASSERT(0 <= k && k < map->dim.z);
    game_map_cell *result = &map->data[k * map->dim.x * map->dim.y + j * map->dim.x + i];
    return result;
}

bool32 game_map_set_ground(game_map *map, int i, int j, int k)
{
    game_map_cell *cell = game_map_get(map, i, j, k);
    bool32 is_cell_empty = cell->tag == GameMap_Empty;
    if (is_cell_empty)
    {
        cell->tag = GameMap_Ground;
    }
    return is_cell_empty;
}

bool32 game_map_set_entity(game_map *map, entity_id eid, int i, int j, int k)
{
    game_map_cell *cell = game_map_get(map, i, j, k);
    bool32 is_cell_empty = cell->tag == GameMap_Empty;
    if (is_cell_empty)
    {
        cell->tag = GameMap_Entity;
        cell->eid = eid;
    }
    return is_cell_empty;
}

void entity_id_array_push(entity_id_array *array, entity_id e)
{
    ASSERT(array);
    ASSERT(array->count < array->capacity);
    if (array->count < array->capacity)
    {
        array->data[array->count] = e;
        array->count += 1;
    }
}

void entity_id_array_remove_eid(entity_id_array *array, entity_id e)
{
    uint index;
    for (index = 0; index < array->count; index++)
        if (array->data[index] == e) break;
    if (index < array->count)
    {
        array->data[index] = array->data[array->count - 1];
        array->count -= 1;
    }
}

entity_id entity_id_array_get(entity_id_array *array, int index)
{
    ASSERT(index < array->count);
    if (index < array->count)
    {
        return array->data[index];
    }
    return INVALID_ENTITY_ID;
}

entity *get_entity(game_state *gs, entity_id eid)
{
    entity *result = NULL;
    if (eid)
    {
        bool32 exists = entity_manager_entity_exists(&gs->em, eid);
        uint32 index = entity_id_index_get(eid);
        if (exists && index < MAX_ENTITIES)
        {
            result = gs->entities + index;
        }
    }
    return result;
}

entity *get_hero(game_state *gs)
{
    entity *result = NULL;
    if (gs->heroes.count > 0)
    {
        result = get_entity(gs, gs->heroes.data[0]);
    }
    return result;
}

void push_entity(game_state *gs, entity_id *out_eid, entity **out_ptr)
{
    entity_id eid = entity_manager_entity_create(&gs->em);
    if (out_eid) *out_eid = eid;
    if (out_ptr) *out_ptr = get_entity(gs, eid);
}

static entity_id_array *choose_entity_array(game_state *gs, uint32 archetype)
{
    if (archetype == Entity_Hero) return &gs->heroes;
    if (archetype == Entity_Monster) return &gs->monsters;
    if (archetype == Entity_Projectile) return &gs->projectiles;
    if (archetype == Entity_UiElement) return &gs->ui_elements;
    return NULL;
}

entity *game_entity_push(game_state *gs, uint32 tag, int x, int y)
{
    entity *e = NULL;
    entity_id_array *array = choose_entity_array(gs, tag);
    if (array->count < array->capacity)
    {
        entity_id eid = INVALID_ENTITY_ID;
        push_entity(gs, &eid, &e);
        if (e)
        {
            int z = gs->map_ground_level + 1;
            e->tag = tag;
            e->tile = vector3i_create(x, y, z);
            e->position = vector3_create((float) x, (float) y, (float) z);

            game_map_set_entity(&gs->map, eid, x, y, z);
            entity_id_array_push(array, eid);
        }
    }
    return e;
}

entity_id ui_element_push(game_state *gs, entity_id parent_id)
{
    entity_id eid = INVALID_ENTITY_ID;
    if (gs->ui_elements.count < gs->ui_elements.capacity)
    {
        entity *e = NULL;
        push_entity(gs, &eid, &e);
        if (e)
        {
            e->tag = Entity_UiElement;
            e->ui.parent = parent_id;
            e->ui.position = vector2_create(100, 100);
            e->ui.width = 100;
            e->ui.height = 100;
            e->ui.scale = vector2_create(1, 1);
            e->ui.rotation = 0.f;

            entity_id_array_push(&gs->ui_elements, eid);
        }
    }
    else
    {
        printf("Error: ui element count exceeded capacity (capacity=%d)\n", gs->ui_elements.capacity);
    }
    return eid;
}

void ui_drawable_push(game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);
    ui_element_flag_set(&e->ui, UiBehaviour_Visible);
    e->ui.is_visible = true;

    entity_id_array_push(&gs->ui_visibles, eid);
}

void ui_hoverable_push(game_state *gs, entity_id eid, float x_min, float y_min, float x_max, float y_max)
{
    entity *e = get_entity(gs, eid);
    ui_element_flag_set(&e->ui, UiBehaviour_Hoverable);
    e->ui.hover_area_min.x = x_min;
    e->ui.hover_area_min.y = y_min;
    e->ui.hover_area_max.x = x_max;
    e->ui.hover_area_max.y = y_max;

    entity_id_array_push(&gs->ui_hoverables, eid);
}

void ui_clickable_push(game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);
    if (ui_element_flag_test(&e->ui, UiBehaviour_Hoverable))
    {
        ui_element_flag_set(&e->ui, UiBehaviour_Clickable);
        entity_id_array_push(&gs->ui_clickables, eid);
    }
    else
    {
        printf("UI Error: trying to make non-hoverable element clickable!\n");
    }
}

void game_move_animation_start(entity *e, float x0, float y0, float t0, float x1, float y1, float t1)
{
    e->move_animation_start_time = t0;
    e->move_animation_end_time = t1;
    e->move_animation_from_x = x0;
    e->move_animation_from_y = y0;
    e->move_animation_to_x = x1;
    e->move_animation_to_y = y1;
}

entity_id game_entity_create(game_state *gs, uint32 archetype)
{
    ASSERT(archetype < Entity_ArchetypeCount);
    entity_id eid = entity_manager_entity_create(&gs->em);
    entity_id_array *array = choose_entity_array(gs, archetype);
    entity_id_array_push(array, eid);
    entity *e = get_entity(gs, eid);
    e->tag = archetype;
    return eid;
}

void game_entity_destroy(game_state *gs, entity_id eid)
{
    entity *e = get_entity(gs, eid);
    entity_id_array *array = choose_entity_array(gs, e->tag);
    entity_id_array_remove_eid(array, eid);
    entity_manager_entity_destroy(&gs->em, eid);
}

entity_id game_entity_create_hero(game_state *gs, int x, int y)
{
    entity_id eid = game_entity_create(gs, Entity_Hero);
    entity *e = get_entity(gs, eid);
    int z = gs->map_ground_level + 1;
    e->tile = vector3i_create(x, y, z);
    e->position = vector3_create((float) x, (float) y, (float) z);
    game_map_set_entity(&gs->map, eid, x, y, z);
    return eid;
}

entity_id game_entity_create_projectile(game_state *gs,
    float32 x0, float32 y0, float32 z0, float64 t0,
    float32 x1, float32 y1, float32 z1, float64 t1)
{
    entity_id eid = game_entity_create(gs, Entity_Projectile);
    entity *e = get_entity(gs, eid);
    e->position = vector3_create(x0, y0, z0);
    game_move_animation_start(e, x0, y0, t0, x1, y1, t1);
    return eid;
}

INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_view game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);

    int i, j;

    memory_allocator game_arena = memory_allocator_arena_create(game_memory.data, game_memory.size, "game");
    game_state *gs = ALLOCATE(game_arena, game_state);
    ctx->game_state = gs;
    gs->game_allocator = game_arena;

    gs->event_count = 0;
    gs->event_capacity = 10;
    gs->events = ALLOCATE_ARRAY(game_arena, event, gs->event_capacity);
    gs->entities = ALLOCATE_ARRAY(game_arena, entity, MAX_ENTITIES);

    gs->heroes = entity_id_array_allocate(game_arena, 1);
    gs->monsters = entity_id_array_allocate(game_arena, 10);
    gs->projectiles = entity_id_array_allocate(game_arena, 10);
    gs->ui_elements = entity_id_array_allocate(game_arena, 10);
    gs->ui_visibles = entity_id_array_allocate(game_arena, 10);
    gs->ui_hoverables = entity_id_array_allocate(game_arena, 10);
    gs->ui_clickables = entity_id_array_allocate(game_arena, 10);

    gs->map.dim = vector3i_create(5, 5, 3);
    gs->map.data = ALLOCATE_ARRAY(game_arena, game_map_cell, gs->map.dim.x * gs->map.dim.y * gs->map.dim.z);
    gs->map_ground_level = 1.f;

    for (j = 0; j < gs->map.dim.y; j++)
    {
        for (i = 0; i < gs->map.dim.x; i++)
        {
            game_map_set_ground(&gs->map, i, j, gs->map_ground_level);
        }
    }

    vector3 map_center = vector3_create(0.5f * gs->map.dim.x, 0.5f * gs->map.dim.y, gs->map_ground_level);
    // vector3_add(map_center, vector3_create(0.f, -1.f, 0.f));

    vector3 at = vector3_create(0, 0, 0);
    gs->camera_default_position = vector3_create(map_center.x, -3.f, 10.f);
    gs->camera_default_forward = vector3_sub(at, gs->camera_default_position);
    gs->camera_default_up = vector3_create(0, 0, 1);
    gs->camera = camera_create_look_at(
        gs->camera_default_position,
        map_center,
        gs->camera_default_up);

    entity_manager_init(&gs->em, game_arena);

    entity_id ui_A = ui_element_push(gs, INVALID_ENTITY_ID);
    entity_id ui_B = ui_element_push(gs, ui_A);
    entity_id ui_C = ui_element_push(gs, ui_A);
    entity_id ui_D = ui_element_push(gs, ui_A);

    entity *e;
    if ((e = get_entity(gs, ui_A)))
    {
        e->ui.position.x = 600;
        e->ui.position.y = 600;
    }
    if ((e = get_entity(gs, ui_B)))
    {
        e->ui.position.x = -100.f;
        e->ui.position.y = 0.f;
        e->ui.width = 90;
        e->ui.height = 90;
        e->ui.color = vector4_create(0.8f, 0.2f, 0.2f, 1.f);
        e->ui_id = EntityUiId_SpellFireball;
    }
    if ((e = get_entity(gs, ui_C)))
    {
        e->ui.position.x = 0.f;
        e->ui.position.y = 0.f;
        e->ui.width = 90;
        e->ui.height = 90;
        e->ui.color = vector4_create(0.2f, 0.2f, 0.8f, 1.f);
    }
    if ((e = get_entity(gs, ui_D)))
    {
        e->ui.position.x = 100.f;
        e->ui.position.y = 0.f;
        e->ui.width = 90;
        e->ui.height = 90;
        e->ui.color = vector4_create(0.2f, 0.8f, 0.2f, 1.f);
    }
    ui_drawable_push(gs, ui_B);
    ui_drawable_push(gs, ui_C);
    ui_drawable_push(gs, ui_D);
    ui_hoverable_push(gs, ui_B, -50, -50, 50, 50);
    ui_clickable_push(gs, ui_B);
    ui_hoverable_push(gs, ui_C, -50, -50, 50, 50);
    ui_clickable_push(gs, ui_C);
    ui_hoverable_push(gs, ui_D, -50, -50, 50, 50);
    ui_clickable_push(gs, ui_D);

    game_entity_create_hero(gs, 3, 3);
}

#include "gameplay/systems.c"

UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_view game_memory, input *input)
{
    game_state *gs = (game_state *) ctx->game_state;
    game_on_every_frame(ctx, gs, input);
}

#if DLL_BUILD
#include <corelibs/math.c>
#include <corelibs/memory/allocator.c>
#include <corelibs/collision.c>

#include <gamelibs/input.c>

#include <engine/game_interface.c>
#endif

#include <gamelibs/camera.c>
#include <gamelibs/entity_manager.c>
#include <gamelibs/ui.c>

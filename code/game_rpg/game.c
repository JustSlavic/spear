#include "game.h"

#include <engine/game_interface.h>

#include <stdio.h>


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

static entity_id_array *choose_entity_array(game_state *gs, uint32 tag)
{
    if (tag == Entity_Hero) return &gs->heroes;
    if (tag == Entity_Monster) return &gs->monsters;
    if (tag == Entity_UiElement) return &gs->ui_elements;
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
            e->tag = tag;
            e->tile = v3i(x, y, 3);
            e->position = v3f(x, y, 3);
            e->move_animation_duration = 0.5f;
            e->move_animation_t = 0.5f;

            game_map_set_entity(&gs->map, eid, x, y, 3);
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
            e->ui.position = v2f(100, 100);
            e->ui.width = 100;
            e->ui.height = 100;
            e->ui.scale = v2f(1, 1);
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

INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_view game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);

    int i, j;

    memory_allocator game_arena = memory_allocator_arena_create(game_memory.data, game_memory.size);
    game_state *gs = ALLOCATE(game_arena, game_state);
    ctx->game_state = gs;
    gs->game_allocator = game_arena;

    gs->entities = ALLOCATE_ARRAY(game_arena, entity, MAX_ENTITIES);

    gs->heroes = entity_id_array_allocate(game_arena, 1);
    gs->monsters = entity_id_array_allocate(game_arena, 10);
    gs->ui_elements = entity_id_array_allocate(game_arena, 10);
    gs->ui_visibles = entity_id_array_allocate(game_arena, 10);
    gs->ui_hoverables = entity_id_array_allocate(game_arena, 10);
    gs->ui_clickables = entity_id_array_allocate(game_arena, 10);

    gs->map.dim = v3i(10, 10, 10);
    gs->map.data = ALLOCATE_ARRAY(game_arena, uint32, gs->map.dim.x * gs->map.dim.y * gs->map.dim.z);

    for (j = 0; j < gs->map.dim.y; j++)
    {
        for (i = 0; i < gs->map.dim.x; i++)
        {
            game_map_set_ground(&gs->map, i, j, 2);
        }
    }

    vector3 at = v3f(0, 0, 0);
    gs->camera_default_position = v3f(4.5, -4, 15);
    gs->camera_default_forward = v3f_sub(at, gs->camera_default_position);
    gs->camera_default_up = v3f(0, 0, 1);
    gs->camera = camera_create_look_at(
        gs->camera_default_position,
        v3f(4.5f, 4.5f, 0.f),
        gs->camera_default_up);

    entity_manager_init(&gs->em, game_arena);

    entity_id ui_A = ui_element_push(gs, INVALID_ENTITY_ID);
    entity_id ui_B = ui_element_push(gs, ui_A);
    entity *A = get_entity(gs, ui_A);
    if (A) { A->ui.position.x = 0; A->ui.position.y = 0; }
    entity *B = get_entity(gs, ui_B);
    if (B) { B->ui.position.x = 100.f; B->ui.position.y = 100.f; }
    ui_drawable_push(gs, ui_B);
    ui_hoverable_push(gs, ui_B, 0, 0, 100, 100);

    entity *hero = game_entity_push(gs, Entity_Hero, 3, 3);
    UNUSED(hero);
}

#include "es/systems.c"

UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_view game_memory, input *input)
{
    game_state *gs = (game_state *) ctx->game_state;
    game_on_every_frame(ctx, gs, input);
}

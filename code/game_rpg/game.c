#include "game.h"

#include <engine/game_interface.h>


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

entity *get_entity(game_state *gs, entity_id eid)
{
    entity *result = NULL;
    if (eid)
    {
        uint32 index = entity_id_index_get(eid);
        if (index < MAX_ENTITIES)
        {
            result = gs->entities + index;
        }
    }
    return result;
}

INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_view game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);

    int i, j;

    memory_allocator game_arena = memory_allocator_arena_create(game_memory.data, game_memory.size);
    game_state *gs = ALLOCATE(game_arena, game_state);
    ctx->game_state = gs;
    gs->game_allocator = game_arena;

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
    gs->hero = entity_manager_entity_create(&gs->em);
    entity *hero = get_entity(gs, gs->hero);
    hero->tag = Entity_Hero;
    hero->position = v3f(3, 3, 3);
    hero->tile_position = v3i(3, 3, 3);
    hero->move_animation_duration = 2.f;
    game_map_set_entity(&gs->map, gs->hero, 3, 3, 3);
}

#include "game_systems.c"

UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_view game_memory, input *input)
{
    game_state *gs = (game_state *) ctx->game_state;
    game_on_every_frame(ctx, gs, input);
}

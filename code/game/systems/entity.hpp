#ifndef GAME_SYSTEMS_SPAWN_ENTITY_CMD
#define GAME_SYSTEMS_SPAWN_ENTITY_CMD

#include <base.h>


namespace game {


entity_action null_action()
{
    entity_action result = {};
    result.kind = ENTITY_ACTION_NONE;
    return result;
}

entity_state idle_state()
{
    entity_state result = {};
    result.kind = ENTITY_STATE_IDLE;
    return result;
}

entity *get_entity(game_state *gs, ecs::entity_id eid)
{
    entity *result = NULL;
    if (eid != ecs::INVALID_ENTITY_ID)
    {
        result = gs->entities + eid.get_index();
    }
    return result;
}

ecs::entity_id get_active_entity_eid(game_state *gs)
{
    ecs::entity_id result = gs->is_in_battle
        ? gs->battle_queue[gs->battle_queue_current_slot]
        : gs->hero_eid;
    return result;
}

entity *get_active_entity(game_state *gs)
{
    ecs::entity_id eid = get_active_entity_eid(gs);
    entity *result = get_entity(gs, eid);
    return result;
}

void remove_entity(game_state *gs, entity *e)
{
    if (e->kind == ENTITY_HERO)
    {
        gs->hero_eid = ecs::INVALID_ENTITY_ID;
    }
    if (e->kind == ENTITY_MONSTER)
    {
        gs->monsters.erase_first_unsorted(e->eid);
    }

    gs->entity_manager.destroy_entity(e->eid);
    gs->battle_queue.erase_first(e->eid);
    gs->battle_queue_current_slot = (gs->battle_queue_current_slot % gs->battle_queue.size());

    if (gs->selected_entity_eid == e->eid)
    {
        gs->selected_entity_eid = get_active_entity_eid(gs);
    }
}

void on_entity_spawned(game_state *gs, entity *e)
{
    if (gs->is_in_battle)
    {
        gs->battle_queue.push_back(e->eid);
    }
}

ecs::entity_id spawn_entity(game_state *gs, int tile_x, int tile_y, int tile_z, entity **p)
{
    ecs::entity_id eid = ecs::INVALID_ENTITY_ID;
    if (gs->map.get(tile_x, tile_y, tile_z) == GameMapOccupation_Empty)
    {
        eid = gs->entity_manager.create_entity();
        auto *entity = gs->entities + eid.get_index();
        entity->eid = eid;
        entity->tile = make_vector3i(tile_x, tile_y, tile_z);
        entity->move_animation_t = 0.f;
        entity->move_animation_end_time = -1.f;
        entity->move_animation_duration = 1.f;

        gs->map.set(tile_x, tile_y, tile_z, GameMapOccupation_Entity);
        printf("Entity eid=%d created on tile (%d, %d, %d)\n", eid.id, tile_x, tile_y, tile_z);
        if (p) *p = entity;
    }
    else
    {
        printf("Error: spawn_entity at (%d, %d, %d); The cell is occupied!\n", tile_x, tile_y, tile_z);
    }
    return eid;
}

ecs::entity_id spawn_hero(game_state *gs, int x, int y, int z)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, z, &e);
    if (e)
    {
        e->kind = ENTITY_HERO;
        e->hp = 3;
        e->max_hp = 5;
        e->invincible = false;
        e->strength = 1;
        e->agility = 1;

        gs->selected_entity_eid = eid;
        gs->hero_eid = eid;
        console::print("hero_eid=%d\n", eid.id);

        on_entity_spawned(gs, e);
    }

    return eid;
}

ecs::entity_id spawn_monster(game_state *gs, int x, int y, int z)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, z, &e);
    if (e)
    {
        e->kind = ENTITY_MONSTER;
        e->hp = 1;
        e->max_hp = 2;
        e->invincible = false;
        e->strength = 1;
        e->agility = 1;

        gs->monsters.push_back(eid);
        console::print("monster_eid=%d\n", eid.id);

        on_entity_spawned(gs, e);
    }

    return eid;
}

ecs::entity_id spawn_stone(game_state *gs, int x, int y, int z)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, z, &e);
    if (e)
    {
        e->kind = ENTITY_STONE;
        e->invincible = true;

        gs->stones.push_back(eid);
        console::print("stone_eid=%d\n", eid.id);
    }
    return eid;
}

bool32 entity_can_move_to(game_state *gs, entity *e, int32 x, int32 y, int32 z)
{
    bool32 result = false;
    if ((0 <= x && x < gs->map.dim.x) &&
        (0 <= y && y < gs->map.dim.y) &&
        (0 <= z && z < gs->map.dim.z))
    {
        if (absolute(e->tile.x - x) &&
            absolute(e->tile.y - y) &&
            absolute(e->tile.z - z))
        {
            result = true;
        }
    }
    return result;
}


} // namespace game


#endif // GAME_SYSTEMS_SPAWN_ENTITY_CMD

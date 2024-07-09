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

bool cell_is_empty(game_state *gs, int x, int y)
{
    if (!gs->is_coords_valid(x, y)) return false;
    return gs->get_map_eid(x, y) == ecs::INVALID_ENTITY_ID;
}

bool cell_is_adjacent_to_entity(entity *e, int x, int y)
{
    return e &&
           ((x == e->x + 1 && y == e->y) ||
            (x == e->x - 1 && y == e->y) ||
            (y == e->y + 1 && x == e->x) ||
            (y == e->y - 1 && x == e->x));
}

bool entity_can_walk_here(game_state *gs, entity *e, int x, int y)
{
    return e && cell_is_empty(gs, x, y) && cell_is_adjacent_to_entity(e, x, y);
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

    gs->set_map_eid(e->x, e->y, ecs::INVALID_ENTITY_ID);
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

ecs::entity_id spawn_entity(game_state *gs, int x, int y, entity **p)
{
    ecs::entity_id eid = ecs::INVALID_ENTITY_ID;
    if (cell_is_empty(gs, x, y))
    {
        eid = gs->entity_manager.create_entity();
        auto *entity = gs->entities + eid.get_index();
        entity->eid = eid;
        entity->x = x;
        entity->y = y;
        gs->set_map_eid(x, y, eid);

        if (p) *p = entity;
    }
    return eid;
}

ecs::entity_id spawn_hero(game_state *gs, int x, int y)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, &e);
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
        console::print("hero_id = %d\n", eid.id);

        on_entity_spawned(gs, e);
    }

    return eid;
}

ecs::entity_id spawn_monster(game_state *gs, int x, int y)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, &e);
    if (e)
    {
        e->kind = ENTITY_MONSTER;
        e->hp = 1;
        e->max_hp = 2;
        e->invincible = false;
        e->strength = 1;
        e->agility = 1;

        gs->monsters.push_back(eid);
        console::print("monster_eid = %d\n", eid.id);

        on_entity_spawned(gs, e);
    }

    return eid;
}

ecs::entity_id spawn_stone(game_state *gs, int x, int y)
{
    entity *e = NULL;
    auto eid = spawn_entity(gs, x, y, &e);
    if (e)
    {
        e->kind = ENTITY_STONE;
        e->invincible = true;

        gs->stones.push_back(eid);
        console::print("stone_eid = %d\n", eid.id);
    }
    return eid;
}


} // namespace game


#endif // GAME_SYSTEMS_SPAWN_ENTITY_CMD

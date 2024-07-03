#ifndef GAME_SYSTEMS_HPP
#define GAME_SYSTEMS_HPP

#include <base.h>
#include <console.hpp>
#include "game.hpp"


namespace game {


void move_entity(game_state *gs, entity *e, int x, int y);


entity *get_entity(game_state *gs, ecs::entity_id eid);

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

ecs::entity_id spawn_entity(game_state *gs, int x, int y, entity **p = NULL);
ecs::entity_id spawn_hero(game_state *gs, int x, int y);
ecs::entity_id spawn_monster(game_state *gs, int x, int y);
ecs::entity_id spawn_stone(game_state *gs, int x, int y);



} // namespace game

#endif // GAME_SYSTEMS_HPP

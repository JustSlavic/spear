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

// entity_action2 get_action2(entity *e)
// {
//     entity_action2 result;
//     result.kind = (e->action.kind == ENTITY_ACTION_MOVE) ? ENTITY_ACTION2_MOVE :
//                   (e->action.kind == ENTITY_ACTION_LEFT_ARM) ? ENTITY_ACTION2_DEFENCE :
//                   (e->action.kind == ENTITY_ACTION_RIGHT_ARM) ? ENTITY_ACTION2_ATTACK :
//                   ENTITY_ACTION2_NONE;
//     result.eid = e->eid;
//     result.x0 = e->x;
//     result.y0 = e->y;
//     result.x1 = e->action.x;
//     result.y1 = e->action.y;
//     return result;
// }

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


void apply_entity_action(game_state *gs, entity *e, entity_action2 action)
{
    if (action.kind == ENTITY_ACTION2_MOVE)
    {
        console::print("%d at (%d, %d) moves to %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
        move_entity(gs, e, action.x1, action.y1);
    }
    else if (action.kind == ENTITY_ACTION2_ATTACK)
    {
        console::print("%d at (%d, %d) attacks %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);

        auto eid = gs->get_map_eid(action.x1, action.y1);
        if (eid == ecs::INVALID_ENTITY_ID) return;

        auto *attacker = e;
        auto *victim = get_entity(gs, eid);

        if (!((victim->state.kind == ENTITY_STATE_DEFENCE) &&
              (victim->state.x == attacker->x) &&
              (victim->state.y == attacker->y)))
        {
            victim->hp -= 1;
        }
    }
    else if (action.kind == ENTITY_ACTION2_DEFENCE)
    {
        console::print("%d at (%d, %d) defences from %d at (%d, %d)\n",
            e->eid.id, action.x0, action.y0,
            gs->get_map_eid(action.x1, action.y1).id, action.x1, action.y1);
        e->state.kind = ENTITY_STATE_DEFENCE;
        e->state.x = action.x1;
        e->state.y = action.y1;
    }
}

// void apply_actions(game_state *gs)
// {
//     console::print("-------- turn %d --------\n", gs->turn_no);
//     for (auto action : gs->action_buffer)
//     {
//         auto *e = get_entity(gs, action.eid);
//         apply_entity_action(gs, e, action);
//         e->action = null_action();
//     }
// }

void reset_entity_states(game_state *gs)
{
    if (gs->hero_eid != ecs::INVALID_ENTITY_ID)
    {
        auto *hero = get_entity(gs, gs->hero_eid);
        hero->state = idle_state();
    }
    for (auto monster_eid : gs->monsters)
    {
        auto *monster = get_entity(gs, monster_eid);
        monster->state = idle_state();
    }
}

void remove_dead_entity(game_state *gs, entity *e)
{
    if (e && e->hp <= 0)
    {
        if (gs->selected_entity_eid == e->eid)
        {
            gs->selected_entity_eid = ecs::INVALID_ENTITY_ID;
        }
        gs->entity_manager.destroy_entity(e->eid);
        if (e->kind == ENTITY_HERO)
        {
            gs->hero_eid = ecs::INVALID_ENTITY_ID;
        }
        else if (e->kind == ENTITY_MONSTER)
        {
            for (int i = 0; i < gs->monsters.size(); i++)
            {
                if (gs->monsters[i] == e->eid)
                {
                    gs->monsters.erase_not_sorted(i);
                    break;
                }
            }
        }
        gs->set_map_eid(e->x, e->y, ecs::INVALID_ENTITY_ID);
    }
}

void remove_dead_entities(game_state *gs)
{
    remove_dead_entity(gs, get_entity(gs, gs->hero_eid));
    for (auto monster_eid : gs->monsters)
    {
        remove_dead_entity(gs, get_entity(gs, monster_eid));
    }
}


} // namespace game

#endif // GAME_SYSTEMS_HPP

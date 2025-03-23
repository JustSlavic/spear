#ifndef GAME_PLANETS_HPP
#define GAME_PLANETS_HPP

#include <base.h>


ecs::entity_id spawn_planet(game_state *gs, vector3 p, vector3 v, float32 r, float32 m)
{
    ecs::entity_id eid = gs->entity_manager.create_entity();
    gs->planets.push_back(eid);

    auto *e = gs->entities + eid.get_index();
    e->position = p;
    e->velocity = v;
    e->radius = r;
    e->mass = m;
    return eid;
}

void move_planets_around_origin(game_state *gs)
{
    for (auto eid : gs->planets)
    {
        auto *e = gs->entities + eid.get_index();
        e->
    }
}

void render_planets(context *ctx, game_state *gs)
{
    for (auto eid : gs->planets)
    {
        auto *e = gs->entities + eid.get_index();
        ctx->render_planet(e->position, e->radius, V4(0.2, 1, 0.8, 1));
    }
}


#endif

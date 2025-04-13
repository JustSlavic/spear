#ifndef GAME_PLANETS_HPP
#define GAME_PLANETS_HPP

#include <base.h>


ecs::entity_id spawn_planet(game_state *gs,
                            vector3 p,
                            vector3 v,
                            float32 r,
                            float32 m,
                            vector3 c)
{
    ecs::entity_id eid = gs->entity_manager.create_entity();
    gs->planets.push_back(eid);

    auto *e = gs->entities + eid.get_index();
    e->position = p;
    e->radius = r;
    e->color = c;
    e->phys_world_handler = gs->phys_world.body_count;

    auto *b = gs->phys_world.bodies + gs->phys_world.body_count++;
    b->X = phys::to_vector(p);
    b->R = phys::matrix::identity();
    b->P = phys::to_vector(v) * m;
    b->L = phys::make_vector(1.0, 0.01, 0.0);
    b->M = m;

    return eid;
}

void move_planets(game_state *gs)
{
    for (auto eid : gs->planets)
    {
        // Copy from phys world ?
        auto *e = gs->entities + eid.get_index();
        auto *b = gs->phys_world.bodies + e->phys_world_handler;
        e->position = phys::to_vector3(b->X);
        e->orientation = phys::to_matrix3(b->R);
    }
}

void render_grid(context *ctx)
{
    {
        auto m = matrix4::scale_x(1000.f) * matrix4::scale_y(0.05f);
        for (int i = -5; i < 6; i++)
        {
            auto c = V4(0.4, 0.4, 0.4, 1);
            auto m1 = matrix4::translate_y((float32) i * 10.f) * m;
            auto m2 = matrix4::translate_y((float32) i * 10.f) * matrix4::rotate_x(to_radians(90)) * m;
            ctx->render_square(m1, c, SHADER_COLOR);
            ctx->render_square(m2, c, SHADER_COLOR);
        }
    }
    {
        auto m = matrix4::scale_x(0.05f) * matrix4::scale_y(1000.f);
        for (int i = -5; i < 6; i++)
        {
            auto c = V4(0.4, 0.4, 0.4, 1);
            auto m1 = matrix4::translate_x((float32) i * 10.f) * m;
            auto m2 = matrix4::translate_x((float32) i * 10.f) * matrix4::rotate_y(to_radians(90)) * m;
            ctx->render_square(m1, c, SHADER_COLOR);
            ctx->render_square(m2, c, SHADER_COLOR);
        }
    }
}

void render_planets(context *ctx, game_state *gs)
{
    for (auto eid : gs->planets)
    {
        auto *e = gs->entities + eid.get_index();
        ctx->render_planet(e->position, e->radius, V4(e->color, 1), quaternion::identity(), e->orientation);
#if 0
        {
            auto m1 = matrix4::identity();
            m1[0].xyz = apply_unit_quaternion(e->orientation, m1[0].xyz);
            m1[1].xyz = apply_unit_quaternion(e->orientation, m1[1].xyz);
            m1[2].xyz = apply_unit_quaternion(e->orientation, m1[2].xyz);
            m1 = matrix4::translate(e->position) *
                 matrix4::scale_y(0.05f) *
                 matrix4::scale(e->radius) *
                 matrix4::translate_x(1);
            ctx->render_square(m1, V4(1, 0, 0, 1), SHADER_COLOR);
        }
        {
            auto m1 = matrix4::identity();
            m1[0].xyz = apply_unit_quaternion(e->orientation, m1[0].xyz);
            m1[1].xyz = apply_unit_quaternion(e->orientation, m1[1].xyz);
            m1[2].xyz = apply_unit_quaternion(e->orientation, m1[2].xyz);
            m1 = matrix4::translate(e->position) *
                 matrix4::scale_x(0.05f) *
                 matrix4::scale(e->radius) *
                 matrix4::translate_y(1);
            ctx->render_square(m1, V4(0, 1, 0, 1), SHADER_COLOR);
        }
        {
            auto m1 = matrix4::translate(e->position);
            m1[0].xyz = apply_unit_quaternion(e->orientation, m1[0].xyz);
            m1[1].xyz = apply_unit_quaternion(e->orientation, m1[1].xyz);
            m1[2].xyz = apply_unit_quaternion(e->orientation, m1[2].xyz);
            // m1 = matrix4::scale_x(0.05f) *
            //      matrix4::scale(e->radius) *
            //      matrix4::translate_z(1) *
            //      matrix4::rotate_y(to_radians(90)) *
            //      matrix4::rotate_x(to_radians(90));
            ctx->render_square(m1, V4(0, 0, 1, 1), SHADER_COLOR);
        }
#endif
    }
    // ctx->render_banner(V3(0), matrix4::scale_x(50), V4(1));
}


#endif

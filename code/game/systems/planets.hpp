#ifndef GAME_PLANETS_HPP
#define GAME_PLANETS_HPP

#include <base.h>

vector3 make_inertia_tensor(float64 a, float64 b, float64 c)
{
    vector3 I = {};
    I._e1 = b*b + c*c;
    I._e2 = a*a + c*c;
    I._e3 = a*a + b*b;

    float32 V = a*b*c;
    return (1.0f/12.0f) * V * I;
}

ecs::entity_id spawn_planet(game_state *gs,
                            vector3 p,
                            vector3 v,
                            float32 r,
                            float32 m,
                            vector3 c)
{
    phys::world *w = &gs->phys_world;

    ecs::entity_id eid = gs->entity_manager.create_entity();
    gs->planets.push_back(eid);

    phys::handle h = phys::create_rigid_body(&gs->phys_world);

    auto *e = gs->entities + eid.get_index();
    e->position = p;
    e->radius = r;
    e->color = c;
    e->phys_world_handle = h;

    vector3 I0 = make_inertia_tensor(2.5f, 5.0f, 0.5f);

    phys::set_position(w, h, p);
    phys::set_orientation(w, h, quaternion::identity());
    phys::set_linear_momentum(w, h, v * m);
    phys::set_angular_momentum(w, h, make_vector3(3.0, 0.01, 0.0));
    phys::set_inertia_tensor(w, h, I0);
    phys::set_mass(w, h, m);

    return eid;
}

void move_planets(game_state *gs)
{
    for (auto eid : gs->planets)
    {
        // Copy from phys world for render
        auto *e = gs->entities + eid.get_index();
        e->position = phys::get_position(&gs->phys_world, e->phys_world_handle);
        e->orientation = phys::get_orientation(&gs->phys_world, e->phys_world_handle);
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
        ctx->render_planet(e->position, e->radius, V4(e->color, 1), e->orientation, matrix3::identity());
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

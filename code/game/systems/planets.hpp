#ifndef GAME_PLANETS_HPP
#define GAME_PLANETS_HPP

#include <base.h>

vector3 make_inertia_tensor(float32 a, float32 b, float32 c)
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

    phys::rigid_body *body = phys::get_rigid_body(w, h);
    body->X = p;
    body->Q = quaternion::identity();
    body->P = v * m;
    body->L = make_vector3(3.0f, 0.01f, 0.0f);
    body->I = I0;
    body->M = m;

    return eid;
}

void move_planets(game_state *gs)
{
    for (auto eid : gs->planets)
    {
        // Copy from phys world for render
        auto *e = gs->entities + eid.get_index();
        auto *b = phys::get_rigid_body(&gs->phys_world, e->phys_world_handle);
        e->position = b->X;
        e->orientation = b->Q;
    }
}

void render_grid(context *ctx)
{
    int grid_n = 20;
    float32 dx = 5.f;
    float32 dy = 5.f;
    {
        for (int i = -grid_n; i <= grid_n; i++)
        {
            auto c = V4(0.2, 0.2, 0.2, 1);
            auto m = matrix4::translate_y((float32) i * dy)
                   * matrix4::scale_x((float32) grid_n * dx)
                   * matrix4::scale_y(0.05f);
            ctx->render_square(m, c, RenderShader_SingleColor);
        }
    }
    {
        for (int i = -grid_n; i <= grid_n; i++)
        {
            auto c = V4(0.2, 0.2, 0.2, 1);
            auto m = matrix4::translate_x((float32) i * dx)
                   * matrix4::scale_x(0.05f)
                   * matrix4::scale_y((float32) grid_n * dy);
            ctx->render_square(m, c, RenderShader_SingleColor);
        }
    }
}

void render_planets(context *ctx, game_state *gs, input_state *input)
{
    for (int planet_index = 0; planet_index < gs->planets.size(); planet_index++)
    {
        auto shader_tag = planet_index == 0 ? RenderShader_Sun : RenderShader_Phong;
        auto eid = gs->planets[planet_index];
        auto *e = gs->entities + eid.get_index();
        ctx->render_planet(e->position, e->radius, V4(e->color, 1), e->orientation, shader_tag);

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
            ctx->render_square(m1, V4(1, 0, 0, 1), RenderShader_SingleColor);
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
            ctx->render_square(m1, V4(0, 1, 0, 1), RenderShader_SingleColor);
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
            ctx->render_square(m1, V4(0, 0, 1, 1), RenderShader_SingleColor);
        }
#endif
    }

    {
        float32 E = 0.f;
        for (uint32 i = 0; i < gs->planets.size(); i++)
        {
            auto *planet = gs->entities + gs->planets[i].get_index();
            auto *body = phys::get_rigid_body(&gs->phys_world, planet->phys_world_handle);

            float32 G = 0.1f;
            float32 K = 0.5f * norm_squared(body->P) / body->M;
            E += K;
        }
        auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
        snprintf((char *) buffer.data, 63, "E = %f", E);
        ctx->render_text(matrix4::translate(10.f, 150.f, 0.f), V4(1), (char const *) buffer.data);
    }

    // ctx->render_banner(V3(0), matrix4::scale_x(50), V4(1));
}


#endif

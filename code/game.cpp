#include <game.hpp>
#include <game_state.hpp>

#include <math/rectangle2.hpp>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>

#include <collision.hpp>

#include <stdio.h>
//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    using namespace math;

    ASSERT(sizeof(game_state) < game_memory.size);
    game_state *gs = (game_state *) game_memory.memory;

    // @note: let zero-indexed entity be 'null entity' representing lack of entity
    gs->entity_count = 1;

    gs->camera_position = V3(0, 0, 8);

    float32 vbo_init[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };

    uint32 ibo_init[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    gfx::vertex_buffer_layout vbl = {};
    gfx::push_layout_element(&vbl, 3);

    auto vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(vbo_init));
    memory::copy(vbo.memory, vbo_init, sizeof(vbo_init));

    auto ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(ibo_init));
    memory::copy(ibo.memory, ibo_init, sizeof(ibo_init));

    gs->rectangle_mesh = create_mesh_resource(&context->resource_storage, vbo, ibo, vbl);
    gs->rectangle_shader = create_shader_resource(&context->resource_storage, STRID("rectangle.shader"));
    gs->circle_shader = create_shader_resource(&context->resource_storage, STRID("circle.shader"));

    // auto *e1 = push_entity(gs);
    // e1->type = ENTITY_CIRCLE;
    // e1->position = V2(0.001, 0.f);
    // e1->velocity = V2(0.0, 0.0);
    // e1->mass = 1.0f;
    // e1->radius = .001f;

    // auto *e2 = push_entity(gs);
    // e2->type = ENTITY_CIRCLE;
    // e2->position = V2(100.0, 0);
    // e2->velocity = V2(-0.5, 0);
    // e2->mass = 1.0f;
    // e2->radius = .20f;

    for (int y = -3; y < 4; y++)
    {
        for (int x = -3; x < 4; x++)
        {
            // comment to find
            auto *entity = push_entity(gs);
            entity->type = ENTITY_CIRCLE;
            entity->position = V2(x, y);
            entity->velocity = -V2(x, y) * 0.01f;
            entity->radius = 0.05f * math::absolute(x) + 0.1f;
            entity->mass = 0.5f * math::absolute(x) + 1.f;
        }
    }
}

math::rectangle2 compute_aabb(entity *e)
{
    math::rectangle2 aabb;
    aabb.center = e->position;

    if (e->type == ENTITY_CIRCLE)
    {
        aabb.radii = V2(e->radius);
    }
    else if (e->type == ENTITY_RECTANGLE)
    {
        auto transform = math::rotated_z(e->rotation, math::matrix4::identity());

        auto lt = V4(e->position.x - e->width * .5f, e->position.y + e->height * .5f, 0, 1);
        auto lb = V4(e->position.x - e->width * .5f, e->position.y - e->height * .5f, 0, 1);
        auto rt = V4(e->position.x + e->width * .5f, e->position.y + e->height * .5f, 0, 1);
        auto rb = V4(e->position.x + e->width * .5f, e->position.y - e->height * .5f, 0, 1);

        lt = lt * transform;
        lb = lb * transform;
        rt = rt * transform;
        rb = rb * transform;

        aabb.radii.x = 0.5f * math::absolute(lt.x - rb.x);
        aabb.radii.y = 0.5f * math::absolute(lt.y - rb.y);

        float32 rx = 0.5f * math::absolute(lb.x - rt.x);
        float32 ry = 0.5f * math::absolute(lb.y - rt.y);

        if (aabb.radii.x < rx) aabb.radii.x = rx;
        if (aabb.radii.y < ry) aabb.radii.y = ry;
    }
    return aabb;
}

//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
// - input_devices input;
// - float32 dt;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{
    using namespace math;

    game_state *gs = (game_state *) game_memory.memory;

    if (get_press_count(input->keyboard_device[keyboard::esc]))
    {
        push_execution_command(context, exit_command());
    }

    vector3 camera_velocity = vector3::zero();
    if (get_hold_count(input->keyboard_device[keyboard::w]))
    {
        camera_velocity.y += 1;
    }
    if (get_hold_count(input->keyboard_device[keyboard::a]))
    {
        camera_velocity.x -= 1;
    }
    if (get_hold_count(input->keyboard_device[keyboard::s]))
    {
        camera_velocity.y -= 1;
    }
    if (get_hold_count(input->keyboard_device[keyboard::d]))
    {
        camera_velocity.x += 1;
    }

    float32 camera_speed = 0.1f;
    gs->camera_position += camera_velocity * camera_speed * dt;

    // Setup camera
    {
        gfx::render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera_position;
        setup_camera.look_at_position = V3(gs->camera_position.x, gs->camera_position.y, 0);
        setup_camera.camera_up_direction = V3(0, 1, 0);

        push_setup_camera_command(context, setup_camera);
    }

    // Background
    {
        gfx::render_command::command_draw_background draw_background;
        draw_background.mesh = gs->rectangle_mesh;
        draw_background.shader = gs->rectangle_shader;
        draw_background.color = math::make_vector4(1.0, 1.0, 1.0, 1.0);
        push_draw_background_command(context, draw_background);
    }

#if 0
    auto gravity = V2(0, -9.8); // m/s^2
#else
    auto gravity = V2(0, 0); // m/s^2
#endif

    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *entity = gs->entities + entity_index;

        float32 dt_ = dt;
        for (int move = 0; move < 4; move++)
        {
            // printf("entity %d move %d:\n", entity_index, move + 1);
            auto acceleration = gravity;

            auto v = entity->velocity;
            entity->velocity = entity->velocity + acceleration * dt_;
            entity->rotation = entity->rotation + entity->rotational_velocity * dt;

            auto old_p = entity->position;
            entity->position = entity->position + v * dt_;
            auto new_p = entity->position;

            math::rectangle2 aabb = compute_aabb(entity);
            entity->aabb = aabb;

            collision_data collision;
            bool32 collided = find_collision_point(gs, entity_index, &collision);
            if (collided)
            {
                auto normal = normalized(collision.point - entity->position);
                float32 overlap_depth = entity->radius - length(collision.point - entity->position);
                ASSERT(overlap_depth >= 0.f);
                entity->position -= overlap_depth * normal;

                auto *entity1 = entity;
                auto *entity2 = get_entity(gs, collision.entity_index);
                auto m1 = entity->mass;
                auto m2 = entity2->mass;

                auto proj_v1 = dot(entity->velocity, normal);
                auto proj_v2 = dot(entity2->velocity, normal);
                auto v1_ = 2.f * m2 / (m1 + m2) * proj_v2 - (m2 - m1) / (m1 + m2) * proj_v1;
                auto v2_ = 2.f * m1 / (m2 + m1) * proj_v1 - (m1 - m2) / (m2 + m1) * proj_v2;

                entity1->velocity = entity1->velocity - (proj_v1 + v1_) * normal;
                entity2->velocity = entity2->velocity - (proj_v2 - v2_) * normal;

                entity1->collided = true;
                entity2->collided = true;
            }

            float32 d = length(old_p - entity->position);
            if (math::absolute(d) < EPSILON) break;
            float32 full_d = length(old_p - new_p);

            // printf("d=%f %%of path passed = %5.2f%%\n", d, d / full_d * 100.f);
            auto ddt = dt_ * (d / full_d);
            dt_ -= ddt;
            // printf("ddt=%f\n", ddt);
            if (dt_ < EPSILON) break;
        }

        // gfx::render_command::command_draw_mesh_with_color draw_aabb;
        // draw_aabb.mesh_token = gs->rectangle_mesh;
        // draw_aabb.shader_token = gs->rectangle_shader;
        // draw_aabb.model =
        //     math::translated(V3(entity->position.x, entity->position.y, 0),
        //     math::scaled(V3(aabb.radii, 1),
        //         math::matrix4::identity()));
        // draw_aabb.color = collided ? V4(0.8f, 0.8f, 0.2f, 1.0f) : V4(0.f, 0.6f, 0.0f, 1.0f);

        // push_draw_mesh_with_color_command(context, draw_aabb);

        gfx::render_command::command_draw_mesh_with_color draw_mesh;
        draw_mesh.mesh_token = gs->rectangle_mesh;
        draw_mesh.shader_token = gs->circle_shader;
        draw_mesh.model =
            math::translated(V3(entity->position.x, entity->position.y, 0),
            math::rotated_z(entity->rotation,
            math::scaled(V3(entity->radius, entity->radius, 1),
                math::matrix4::identity())));
        draw_mesh.color = entity->collided ? V4(1.f, 0.f, 0.f, 1.f) : V4(0.1f + 0.05f * entity_index, 0.32f, 0.72f, 1.0f);

        push_draw_mesh_with_color_command(context, draw_mesh);

        entity->collided = false;
    }

#if 0
    double energy = 0;
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *e = get_entity(gs, entity_index);
        energy += (0.5f * e->mass * math::length_squared(e->velocity));
    }

    printf("energy=%lf\n", energy);
#endif
}


#include <memory/allocator.cpp>
#include <string_id.cpp>

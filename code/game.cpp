#include <game.hpp>
#include <game_interface.hpp>

#include <math/integer.hpp>
#include <math/float64.hpp>
#include <math/rectangle2.hpp>
#include <math/vector4.hpp>
#include <math/matrix2.hpp>
#include <math/matrix4.hpp>

#include <collision.hpp>

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
    world *w = &gs->world;

    w->chunk_width = 1.0f;
    w->chunk_height = 1.0f;

    memory::initialize_memory_arena(&gs->game_allocator, (byte *) game_memory.memory + sizeof(game_state), game_memory.size - sizeof(game_state));

    gs->entities = (entity *) ALLOCATE_BUFFER_(&gs->game_allocator, sizeof(entity) * 200000);
    gs->entities_capacity = 200000;

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

#if 0
    auto *e1 = push_entity(gs);
    e1->type = ENTITY_CIRCLE;
    e1->position = V2(0, .3);
    e1->velocity = V2(.001, -.002) * 10;
    e1->mass = 1.0f;
    e1->radius = .05f;

    auto *e2 = push_entity(gs);
    e2->type = ENTITY_CIRCLE;
    e2->position = V2(0.3, 0);
    e2->velocity = V2(-0.001, 0);
    e2->mass = 100.f;
    e2->radius = .25f;
#endif

    entity_ref border1 = push_entity(gs);
    border1.entity->type = ENTITY_ALIGNED_RECTANGLE;
    border1.entity->position = V2(-4, 0);
    border1.entity->height = 5.5f;
    border1.entity->width = 0.1f;
    border1.entity->mass = 1000000.f;
    put_entity_in_chunk(gs, w, border1.eid);

    entity_ref border2 = push_entity(gs);
    border2.entity->type = ENTITY_ALIGNED_RECTANGLE;
    border2.entity->position = V2(0, 2.4);
    border2.entity->height = 0.1f;
    border2.entity->width = 8.5f;
    border2.entity->mass = 1000000.f;
    put_entity_in_chunk(gs, w, border2.eid);

    entity_ref border3 = push_entity(gs);
    border3.entity->type = ENTITY_ALIGNED_RECTANGLE;
    border3.entity->position = V2(0, -2.4);
    border3.entity->height = 0.1f;
    border3.entity->width = 8.5f;
    border3.entity->mass = 1000000.f;
    put_entity_in_chunk(gs, w, border3.eid);

    entity_ref border4 = push_entity(gs);
    border4.entity->type = ENTITY_ALIGNED_RECTANGLE;
    border4.entity->position = V2(4, 0);
    border4.entity->height = 5.5f;
    border4.entity->width = 0.1f;
    border4.entity->mass = 1000000.f;
    put_entity_in_chunk(gs, w, border4.eid);

    auto e1 = push_entity(gs);
    e1.entity->type = ENTITY_CIRCLE;
    e1.entity->position = V2(-3, 0);
    e1.entity->velocity = V2(0.1, 0);
    e1.entity->mass = 5.f;
    e1.entity->radius = .05f;
    put_entity_in_chunk(gs, w, e1.eid);

    // auto *e2 = push_entity(gs);
    // e2->type = ENTITY_CIRCLE;
    // e2->position = V2(0, 0);
    // e2->velocity = V2(0, 0);
    // e2->mass = 5.f;
    // e2->radius = .05f;

    // auto *e3 = push_entity(gs);
    // e3->type = ENTITY_CIRCLE;
    // e3->position = V2(0, -1);
    // e3->velocity = V2(0, 0);
    // e3->mass = 5.f;
    // e3->radius = .05f;

    for (int y = -15; y < 15; y++)
    {
        for (int x = -10; x < 11; x++)
        {
            // if (x > -2 && x < 2) continue;
            // comment to find
            entity_ref particle = push_entity(gs);
            particle.entity->type = ENTITY_CIRCLE;
            particle.entity->position = V2(x + 0.001 * y, y) * 0.201f;
            particle.entity->velocity = V2(-x, -y) * 0.1f;
            particle.entity->radius = .025f;
            particle.entity->mass = 0.05f;

            put_entity_in_chunk(gs, w, particle.eid);
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
    else if (e->type == ENTITY_ALIGNED_RECTANGLE)
    {
        aabb.radii = V2(e->width * 0.5f, e->height * 0.5f);
    }
    else if (e->type == ENTITY_ORIENTED_RECTANGLE)
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


void draw_aligned_rectangle(execution_context *context, game_state *gs, float32 x, float32 y, float32 half_width, float32 half_height, math::vector4 color)
{
    gfx::render_command::command_draw_mesh_with_color draw_aligned_rectangle;
    draw_aligned_rectangle.mesh_token = gs->rectangle_mesh;
    draw_aligned_rectangle.shader_token = gs->rectangle_shader;
    draw_aligned_rectangle.model =
        math::translated(V3(x, y, 0),
        math::scaled(V3(half_width, half_height, 1),
            math::matrix4::identity()));
    draw_aligned_rectangle.color = color;

    push_draw_mesh_with_color_command(context, draw_aligned_rectangle);
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
    world *w = &gs->world;

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

    // Grid
    {
        for (int32 x = -5; x < 5; x++)
        {
            float32 x_ = (float32) x + 0.5f;
            draw_aligned_rectangle(context, gs, x_, 0.f, 0.01f, 3.f, V4(0.9f, 0.9f, 0.9f, 1.0f));
        }
        for (int32 y = -5; y < 5; y++)
        {
            float32 y_ = (float32) y + 0.5f;
            draw_aligned_rectangle(context, gs, 0.f, y_, 3.f, 0.01f, V4(0.9f, 0.9f, 0.9f, 1.0f));
        }
    }

    for (uint32 eid = 1; eid < gs->entity_count; eid++)
    {
        auto *e = get_entity(gs, eid);

        int32 chunk_x, chunk_y;
        get_chunk_coordinates(w, e->position, &chunk_x, &chunk_y);

        world_chunk **slot = get_world_chunk_slot(gs, w, 0, 0);

        bool32 inside = false;
        if (slot && (*slot) && (*slot)->entity_count > 0) inside = true;

        draw_aligned_rectangle(context, gs, 0.f, 0.f, 0.5f, 0.5f,
            inside ? V4(0.f, 1.f, 0.f, 1.f) : V4(1.f, 0.f, 0.f, 1.f));
    }

    // Coordinates
    {
        // X axis
        draw_aligned_rectangle(context, gs, 0.5f, 0.f, 0.5f, 0.005f, V4(0.9, 0.2, 0.2, 1.0));
        // Y axis
        draw_aligned_rectangle(context, gs, 0.f, 0.5f, 0.005f, 0.5f, V4(0.2, 0.9, 0.2, 1.0));
    }

#if 1
    auto gravity = V2(0, -0.0098); // m/s^2
#else
    auto gravity = V2(0, 0); // m/s^2
#endif

    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        entity *e = get_entity(gs, entity_index);
        if (e->deleted) continue;

        e->collided = false;

        float32 dt_ = dt; // * 0.05f;
        for (int move = 0; move < 5; move++)
        {
            auto acceleration = gravity;
            if (e->type == ENTITY_ALIGNED_RECTANGLE) acceleration = V2(0);

            auto old_v = e->velocity;
            e->velocity = e->velocity + acceleration * dt_;
            e->rotation = e->rotation + e->rotational_velocity * dt;

            auto old_p = e->position;
            auto new_p = e->position + old_v * dt_;

            math::rectangle2 aabb = compute_aabb(e);
            e->aabb = aabb;

            auto full_distance = 0.f;
            auto direction = normalized(new_p - old_p, &full_distance);
            auto distance = full_distance;
            if (distance == 0) continue;

            collision_data collision = no_collision();

            int32 old_chunk_x, old_chunk_y;
            get_chunk_coordinates(w, old_p, &old_chunk_x, &old_chunk_y);
            int32 new_chunk_x, new_chunk_y;
            get_chunk_coordinates(w, old_p, &new_chunk_x, &new_chunk_y);

            int32 min_chunk_x = min_(old_chunk_x, new_chunk_x) - 1;
            int32 min_chunk_y = min_(old_chunk_y, new_chunk_y) - 1;
            int32 max_chunk_x = max_(old_chunk_x, new_chunk_x) + 1;
            int32 max_chunk_y = max_(old_chunk_y, new_chunk_y) + 1;

            for (int32 chunk_x = min_chunk_x; chunk_x <= max_chunk_x; chunk_x++)
            {
                for (int32 chunk_y = min_chunk_y; chunk_y <= max_chunk_y; chunk_y++)
                {
                    world_chunk **slot = get_world_chunk_slot(gs, w, chunk_x, chunk_y);
                    if (slot == NULL) continue;

                    world_chunk *chunk = *slot;

                    while (chunk)
                    {
                        for (uint32 index_in_chunk = 0; index_in_chunk < chunk->entity_count; index_in_chunk++)
                        {
                            uint32 test_eid = chunk->entities[index_in_chunk];

                            // Do not self collide!
                            if (entity_index == test_eid) continue;

                            entity *test_entity = get_entity(gs, test_eid);

                            if ((e->type == ENTITY_CIRCLE) && (test_entity->type == ENTITY_CIRCLE))
                            {
                                if (dot(direction, test_entity->position - e->position) < 0.f) continue;
                                if (dot(test_entity->position - e->position, test_entity->velocity - e->velocity) > 0.f) continue;

                                float32 r = 0.f;
                                bool32 collided = test_ray_sphere(old_p, direction, test_entity->position, e->radius + test_entity->radius, &r);
                                if (collided && r < distance && r < collision.t_in_meters)
                                {
                                    collision.entity1 = e;
                                    collision.entity2 = test_entity;
                                    collision.point = test_entity->position + normalized(e->position - test_entity->position) * test_entity->radius;
                                    collision.normal = normalized(e->position - test_entity->position);
                                    collision.t_in_meters = r;
                                }
                            }
                            else if ((e->type == ENTITY_CIRCLE) && (test_entity->type == ENTITY_ALIGNED_RECTANGLE))
                            {
                                auto tl = top_left(test_entity->aabb);
                                auto bl = bottom_left(test_entity->aabb);
                                auto tr = top_right(test_entity->aabb);
                                auto br = bottom_right(test_entity->aabb);

                                math::vector2 vertices[4] = { br, tr, tl, bl };

                                for (uint32 i = 0; i < ARRAY_COUNT(vertices); i++)
                                {
                                    auto ray1 = old_p;
                                    auto ray2 = new_p;
                                    auto cap1 = vertices[i];
                                    auto cap2 = vertices[(i + 1) % ARRAY_COUNT(vertices)];
                                    auto cap_radius = e->radius;

                                    auto side = cap2 - cap1;
                                    if (dot(V2(-side.y, side.x), ray2 - ray1) >= 0.f) continue;

                                    collision_data temp_collision = {};

                                    float32 t1, t2;
                                    math::vector2 c1, c2;
                                    float32 sq_distance = sq_distance_segment_segment(
                                        ray1, ray2, cap1, cap2,
                                        t1, t2, c1, c2);

                                    if (sq_distance < math::square(cap_radius))
                                    {
                                        float32 r = 0.f;
                                        bool32 collided = test_segment_sphere(ray1, ray2, c2, cap_radius, &r);
                                        ASSERT(collided);

                                        temp_collision.entity1 = e;
                                        temp_collision.entity2 = test_entity;
                                        temp_collision.point = ray1 + normalized(ray2 - ray1) * r;
                                        temp_collision.normal = math::rotated(90_degrees, normalized(cap2 - cap1));
                                        temp_collision.t_in_meters = r;

                                        if (temp_collision.t_in_meters < collision.t_in_meters)
                                        {
                                            collision = temp_collision;
                                        }
                                    }
                                }
                            }
                        }
                        chunk = chunk->next;
                    }
                }
            }

            if (collision.t_in_meters < math::infinity)
            {
                new_p = old_p + collision.t_in_meters * direction + 2.0f * EPSILON * collision.normal;
                direction = normalized(new_p - old_p, &distance);

                auto m1 = collision.entity1->mass;
                auto m2 = collision.entity2->mass;

                auto p1 = m1 * collision.entity1->velocity;
                auto p2 = m2 * collision.entity2->velocity;

                auto proj_p1 = dot(p1, collision.normal);
                auto proj_p2 = dot(p2, collision.normal);

                auto tangent_p1 = p1 - proj_p1 * collision.normal;
                auto tangent_p2 = p2 - proj_p2 * collision.normal;

                auto proj_p1_ = ((m1 - m2) * proj_p1 + 2.0f * m1 * proj_p2) / (m1 + m2);
                auto proj_p2_ = (2.0f * m2 * proj_p1 - (m1 - m2) * proj_p2) / (m1 + m2);

                auto p1_ = (0.6f * proj_p1_ * collision.normal + tangent_p1);
                auto p2_ = (0.6f * proj_p2_ * collision.normal + tangent_p2);

                collision.entity1->velocity = p1_ / m1;
                collision.entity2->velocity = p2_ / m2;

                collision.entity1->collided = true;
                collision.entity2->collided = true;
            }

            move_entity_between_chunks(gs, w, entity_index, old_p, new_p);

            e->position = new_p;
            auto ddt = dt_ * (distance / full_distance);
            dt_ -= ddt;
            if (dt_ < EPSILON) break;
        }

        switch (e->type)
        {
            case ENTITY_CIRCLE:
            {
                gfx::render_command::command_draw_mesh_with_color draw_mesh;
                draw_mesh.mesh_token = gs->rectangle_mesh;
                draw_mesh.shader_token = gs->circle_shader;
                draw_mesh.model =
                    math::translated(V3(e->position.x, e->position.y, 0),
                    math::rotated_z(e->rotation,
                    math::scaled(V3(e->radius, e->radius, 1),
                        math::matrix4::identity())));
                // draw_mesh.color = e->collided ? V4(1.f, 0.f, 0.f, 1.f) : V4(0.1f + 0.01f * entity_index, 0.32f, 0.72f, 1.0f);
                auto c = e->velocity * 100.0f;
                draw_mesh.color = e->collided ? V4(1.f, 0.f, 0.f, 1.f) : V4(0.3 * length(c), 0.2, length(c) * 0.9, 1.f);
                // draw_mesh.color = V4(0.3 * length(c), 0.2, length(c) * 0.9, 1.f);

                push_draw_mesh_with_color_command(context, draw_mesh);
            }
            break;

            case ENTITY_ALIGNED_RECTANGLE:
            {
                draw_aligned_rectangle(context, gs, e->position.x, e->position.y, e->aabb.radii.x, e->aabb.radii.y, e->collided ? V4(0.8f, 0.8f, 0.2f, 1.0f) : V4(0.f, 0.6f, 0.0f, 1.0f));
            }
            break;

            case ENTITY_ORIENTED_RECTANGLE:
            break;

            case ENTITY_INVALID:
                ASSERT_FAIL();
            break;
        }
    }

#if 0
    gs->energy_last_frame = gs->energy;
    gs->energy = 0;
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *e = get_entity(gs, entity_index);
        gs->energy += (0.5f * e->mass * math::length_squared(e->velocity));
    }
    // ASSERT(math::absolute(gs->energy_last_frame - gs->energy) < EPSILON);
    // printf("energy=%lf\n", gs->energy);
#endif
}


#include <memory/allocator.cpp>
#include <string_id.cpp>

#include <game.hpp>
#include <game_interface.hpp>

#include <math/integer.hpp>
#include <math/float64.hpp>
#include <math/rectangle2.hpp>
#include <math/vector4.hpp>
#include <math/matrix2.hpp>
#include <math/matrix4.hpp>

#include <collision.hpp>


#if DEBUG
debug_time_measurement *global_debug_measurements;
uint32 global_debug_call_depth;
#endif


uint32 chunk_hash(uint32 chunk_x, uint32 chunk_y)
{
    uint32 result = chunk_x * 2281 + chunk_y * 5023;
    return result;
}

void get_chunk_coordinates(world *w, math::vector2 p, int32 *chunk_x, int32 *chunk_y)
{
    *chunk_x = math::round_to_int32(p.x / w->chunk_width + EPSILON);
    *chunk_y = math::round_to_int32(p.y / w->chunk_height + EPSILON);
}

game::camera *get_current_camera(game_state *gs)
{
    game::camera *result = gs->cameras.data + gs->current_camera_index;
    return result;
}

INLINE entity_ref push_entity(game_state *gs)
{
    ASSERT(gs->entity_count < gs->entities_capacity);
    entity_ref result;
    result.eid = (uint32) gs->entity_count++;
    result.e = gs->entities + result.eid;
    return result;
}

INLINE entity *get_entity(game_state *gs, uint32 eid)
{
    ASSERT(eid < gs->entity_count);

    entity *result = gs->entities + eid;
    return result;
}

world_chunk *get_new_world_chunk(game_state *gs, world *w)
{
    world_chunk *result = NULL;
    if (w->free_list)
    {
        result = w->free_list;
        w->free_list = w->free_list->next;
        memory::set(result, 0, sizeof(world_chunk));
    }
    else
    {
        result = ALLOCATE(&gs->game_allocator, world_chunk);
    }
    return result;
}

void release_world_chunk(world *w, world_chunk *chunk)
{
    chunk->next = w->free_list;
    w->free_list = chunk;
}

world_chunk **get_world_chunk_slot(game_state *gs, world *w, int32 chunk_x, int32 chunk_y, bool32 create_slot)
{
    DEBUG_BEGIN_TIME_MEASUREMENT(get_world_chunk_slot);
    world_chunk **slot = NULL;

    uint32 hash = chunk_hash(chunk_x, chunk_y);
    for (uint32 offset = 0; offset < ARRAY_COUNT(w->hash_table); offset++)
    {
        uint32 index = (hash + offset) % ARRAY_COUNT(w->hash_table);
        if (w->hash_table[index] == NULL)
        {
            slot = w->hash_table + index;
            if (create_slot)
            {
                auto *new_chunk = get_new_world_chunk(gs, w);
                new_chunk->x = chunk_x;
                new_chunk->y = chunk_y;

                *slot = new_chunk;
            }
            break;
        }
        else if ((w->hash_table[index]->x == chunk_x) && (w->hash_table[index]->y == chunk_y))
        {
            slot = w->hash_table + index;
            break;
        }
    }
    DEBUG_END_TIME_MEASUREMENT(get_world_chunk_slot);
    return slot;
}


void push_entity_in_world_chunk_slot(game_state *gs, world *w, world_chunk **slot, uint32 eid, int32 chunk_x, int32 chunk_y)
{
    DEBUG_BEGIN_TIME_MEASUREMENT(push_entity_in_world_chunk_slot);

    world_chunk *chunk = *slot;

    bool32 found = false;
    while (chunk)
    {
        for (uint32 entity_index = 0; entity_index < chunk->entity_count; entity_index++)
        {
            if (chunk->entities[entity_index] == eid)
            {
                // Entity already in this chunk, do nothing.
                found = true;
                break;
            }
        }
        if (chunk->next == NULL) break;
        chunk = chunk->next;
    }

    if (!found)
    {
        if (chunk->entity_count == ARRAY_COUNT(chunk->entities))
        {
            world_chunk *next_chunk = get_new_world_chunk(gs, w);
            next_chunk->x = chunk_x;
            next_chunk->y = chunk_y;

            chunk->next = next_chunk;
            chunk = chunk->next;
        }
        chunk->entities[chunk->entity_count++] = eid;
    }

    DEBUG_END_TIME_MEASUREMENT(push_entity_in_world_chunk_slot);
}

void remove_entity_from_world_chunk_slot(game_state *gs, world *w, world_chunk **slot, uint32 eid, int32 chunk_x, int32 chunk_y)
{
    DEBUG_BEGIN_TIME_MEASUREMENT(remove_entity_from_world_chunk_slot);

    world_chunk *chunk = *slot;
    world_chunk *prev_chunk = NULL;

    while (chunk)
    {
        bool32 found = false;
        for (uint32 index_in_chunk = 0; index_in_chunk < chunk->entity_count; index_in_chunk++)
        {
            if (chunk->entities[index_in_chunk] == eid)
            {
                found = true;

                // Found entity in this chunk, remove it.
                if (chunk->entity_count == 1)
                {
                    if (prev_chunk)
                    {
                        // Store this chunk in the world's free list
                        prev_chunk->next = chunk->next;
                    }
                    else
                    {
                        // Remove chunk from hash table
                        *slot = chunk->next;
                    }

                    release_world_chunk(w, chunk);
                }
                else
                {
                    // Replace the current eid with the last eid in this chunk.
                    chunk->entities[index_in_chunk] = chunk->entities[chunk->entity_count - 1];
                    chunk->entity_count -= 1;
                }

                break;
            }
        }

        if (not found)
        {
            prev_chunk = chunk;
            chunk = chunk->next;
        }
        else
        {
            break;
        }
    }

    DEBUG_END_TIME_MEASUREMENT(remove_entity_from_world_chunk_slot);
}


void put_entity_in_chunk(game_state *gs, world *w, entity_ref ref)
{
    DEBUG_BEGIN_TIME_MEASUREMENT(put_entity_in_chunk);

    auto min_corner = get_min(ref.e->aabb);
    auto max_corner = get_max(ref.e->aabb);

    int32 min_chunk_x, min_chunk_y;
    get_chunk_coordinates(w, min_corner, &min_chunk_x, &min_chunk_y);

    int32 max_chunk_x, max_chunk_y;
    get_chunk_coordinates(w, max_corner, &max_chunk_x, &max_chunk_y);

    for (int32 chunk_x = min_chunk_x; chunk_x <= max_chunk_x; chunk_x++)
    {
        for (int32 chunk_y = min_chunk_y; chunk_y <= max_chunk_y; chunk_y++)
        {
            world_chunk **slot = get_world_chunk_slot(gs, w, chunk_x, chunk_y, true);

            if (slot)
            {
                ASSERT(*slot);
                push_entity_in_world_chunk_slot(gs, w, slot, ref.eid, chunk_x, chunk_y);
            }
            else
            {
                ASSERT_FAIL(Not enough slots in the hash table);
            }
        }
    }

    DEBUG_END_TIME_MEASUREMENT(put_entity_in_chunk);
}


void move_entity_between_chunks(game_state *gs, world *w, uint32 eid, math::rect2 old_aabb, math::rect2 new_aabb)
{
    DEBUG_BEGIN_TIME_MEASUREMENT(move_entity_between_chunks);

    auto old_min_corner = get_min(old_aabb);
    auto old_max_corner = get_max(old_aabb);

    auto new_min_corner = get_min(new_aabb);
    auto new_max_corner = get_max(new_aabb);

    int32 old_min_chunk_x, old_min_chunk_y;
    get_chunk_coordinates(w, old_min_corner, &old_min_chunk_x, &old_min_chunk_y);

    int32 old_max_chunk_x, old_max_chunk_y;
    get_chunk_coordinates(w, old_max_corner, &old_max_chunk_x, &old_max_chunk_y);

    int32 new_min_chunk_x, new_min_chunk_y;
    get_chunk_coordinates(w, new_min_corner, &new_min_chunk_x, &new_min_chunk_y);

    int32 new_max_chunk_x, new_max_chunk_y;
    get_chunk_coordinates(w, new_max_corner, &new_max_chunk_x, &new_max_chunk_y);

    auto min_chunk_x = min_(old_min_chunk_x, new_min_chunk_x);
    auto min_chunk_y = min_(old_min_chunk_y, new_min_chunk_y);

    auto max_chunk_x = max_(old_max_chunk_x, new_max_chunk_x);
    auto max_chunk_y = max_(old_max_chunk_y, new_max_chunk_y);

    for (int32 chunk_x = min_chunk_x; chunk_x <= max_chunk_x; chunk_x++)
    {
        for (int32 chunk_y = min_chunk_y; chunk_y <= max_chunk_y; chunk_y++)
        {
            bool32 is_inside_old = (old_min_chunk_x <= chunk_x) && (chunk_x <= old_max_chunk_x)
                                && (old_min_chunk_y <= chunk_y) && (chunk_y <= old_max_chunk_y);
            bool32 is_inside_new = (new_min_chunk_x <= chunk_x) && (chunk_x <= new_max_chunk_x)
                                && (new_min_chunk_y <= chunk_y) && (chunk_y <= new_max_chunk_y);

            if ((is_inside_old && is_inside_new) || (not is_inside_old && not is_inside_new))
                continue;
            else if (is_inside_old and not is_inside_new)
            {
                world_chunk **slot = get_world_chunk_slot(gs, w, chunk_x, chunk_y);
                remove_entity_from_world_chunk_slot(gs, w, slot, eid, chunk_x, chunk_y);
            }
            else if (not is_inside_old and is_inside_new)
            {
                world_chunk **slot = get_world_chunk_slot(gs, w, chunk_x, chunk_y, true);
                ASSERT_MSG(slot, "Not enough slots in the hash table!");
                if (slot)
                {
                    ASSERT_MSG(*slot, "If slot not null, but points to null, probably memory is depleted!");
                    if (*slot)
                    {
                        push_entity_in_world_chunk_slot(gs, w, slot, eid, chunk_x, chunk_y);
                    }
                }
            }
        }
    }

    DEBUG_END_TIME_MEASUREMENT(move_entity_between_chunks);
}


math::rectangle2 compute_aabb(entity *e, math::vector2 entity_position)
{
    math::rectangle2 aabb;
    aabb.center = entity_position;

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

        auto lt = V4(entity_position.x - e->width * .5f, entity_position.y + e->height * .5f, 0, 1);
        auto lb = V4(entity_position.x - e->width * .5f, entity_position.y - e->height * .5f, 0, 1);
        auto rt = V4(entity_position.x + e->width * .5f, entity_position.y + e->height * .5f, 0, 1);
        auto rb = V4(entity_position.x + e->width * .5f, entity_position.y - e->height * .5f, 0, 1);

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
    render_command::command_draw_mesh_with_color draw_aligned_rectangle;
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
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    using namespace math;

    global_debug_measurements = context->debug_measurements;

    ASSERT(sizeof(game_state) < game_memory.size);
    game_state *gs = (game_state *) game_memory.memory;
    world *w = &gs->w;

    w->chunk_width  = .250f;
    w->chunk_height = .250f;

    memory::initialize_memory_arena(&gs->game_allocator, (byte *) game_memory.memory + sizeof(game_state), game_memory.size - sizeof(game_state));

    gs->entities = (entity *) ALLOCATE_BUFFER_(&gs->game_allocator, sizeof(entity) * 200000);
    gs->entities_capacity = 200000;

    // @note: let zero-indexed entity be 'null entity' representing lack of entity
    gs->entity_count = 1;

    gs->cameras = ALLOCATE_ARRAY(&gs->game_allocator, game::camera, 5);
    gs->cameras.push_back({V3(0, 0, 8)});
    gs->cameras.push_back({V3(0, 0, 12)});
    gs->current_camera_index = 0;

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
    border1.e->type = ENTITY_ALIGNED_RECTANGLE;
    border1.e->position = V2(-4, 0);
    border1.e->height = 5.5f;
    border1.e->width = 0.1f;
    // border1.e->position = V2(0);
    // border1.e->width = 1.0f;
    // border1.e->height = 1.0f;
    border1.e->mass = 1000000.f;
    border1.e->aabb = compute_aabb(border1.e, border1.e->position);
    put_entity_in_chunk(gs, w, border1);

    entity_ref border2 = push_entity(gs);
    border2.e->type = ENTITY_ALIGNED_RECTANGLE;
    border2.e->position = V2(0, 2.4);
    border2.e->height = 0.1f;
    border2.e->width = 8.5f;
    border2.e->mass = 1000000.f;
    border2.e->aabb = compute_aabb(border2.e, border2.e->position);
    put_entity_in_chunk(gs, w, border2);

    entity_ref border3 = push_entity(gs);
    border3.e->type = ENTITY_ALIGNED_RECTANGLE;
    border3.e->position = V2(0, -2.4);
    border3.e->height = 0.1f;
    border3.e->width = 8.5f;
    border3.e->mass = 1000000.f;
    border3.e->aabb = compute_aabb(border3.e, border3.e->position);
    put_entity_in_chunk(gs, w, border3);

    entity_ref border4 = push_entity(gs);
    border4.e->type = ENTITY_ALIGNED_RECTANGLE;
    border4.e->position = V2(4, 0);
    border4.e->height = 5.5f;
    border4.e->width = 0.1f;
    border4.e->mass = 1000000.f;
    border4.e->aabb = compute_aabb(border4.e, border4.e->position);
    put_entity_in_chunk(gs, w, border4);

    // auto e1 = push_entity(gs);
    // e1.e->type = ENTITY_CIRCLE;
    // e1.e->position = V2(0, 0);
    // e1.e->velocity = V2(0, 0);
    // e1.e->mass = 5.f;
    // e1.e->radius = .4f;
    // e1.e->aabb = compute_aabb(e1.e, e1.e->position);
    // put_entity_in_chunk(gs, w, e1);

    // auto e2 = push_entity(gs);
    // e2.e->type = ENTITY_CIRCLE;
    // e2.e->position = V2(0, 1);
    // e2.e->velocity = V2(0, 0);
    // e2.e->mass = 5.f;
    // e2.e->radius = .4f;
    // e2.e->aabb = compute_aabb(e2.e, e2.e->position);
    // put_entity_in_chunk(gs, w, e2);

    // auto e3 = push_entity(gs);
    // e3.e->type = ENTITY_CIRCLE;
    // e3.e->position = V2(0, 2);
    // e3.e->velocity = V2(0, 0);
    // e3.e->mass = 5.f;
    // e3.e->radius = .4f;
    // e3.e->aabb = compute_aabb(e3.e, e3.e->position);
    // put_entity_in_chunk(gs, w, e3);


    for (int y = -10; y < 11; y++)
    {
        for (int x = -30; x < 31; x++)
        {
            // if (x > -2 && x < 2) continue;
            // comment to find
            entity_ref particle = push_entity(gs);
            particle.e->type = ENTITY_CIRCLE;
            particle.e->position = V2(x, y) * 0.1f;
            particle.e->velocity = V2(x, y + 20.) * 0.01f;
            particle.e->radius = .025f;
            particle.e->mass = 0.05f;
            particle.e->aabb = compute_aabb(particle.e, particle.e->position);

            put_entity_in_chunk(gs, w, particle);
        }
    }
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

    global_debug_measurements = context->debug_measurements;

    DEBUG_BEGIN_TIME_MEASUREMENT(update_and_render);

    game_state *gs = (game_state *) game_memory.memory;
    world *w = &gs->w;

    if (get_press_count(input->keyboard_device[keyboard::esc]))
    {
        push_execution_command(context, exit_command());
    }

    if (get_press_count(input->keyboard_device[keyboard::y]))
    {
        gs->current_camera_index = (gs->current_camera_index + 1) % gs->cameras.size;
    }

    game::camera *current_camera = get_current_camera(gs);

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

    float32 camera_speed = .5f;
    current_camera->position += camera_velocity * camera_speed * dt;

    // Setup camera
    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = current_camera->position;
        setup_camera.look_at_position = V3(current_camera->position.x, current_camera->position.y, 0);
        setup_camera.camera_up_direction = V3(0, 1, 0);

        push_setup_camera_command(context, setup_camera);
    }

    // Background
    {
        render_command::command_draw_background draw_background;
        draw_background.mesh = gs->rectangle_mesh;
        draw_background.shader = gs->rectangle_shader;
        draw_background.color = math::make_vector4(1.0, 1.0, 1.0, 1.0);
        push_draw_background_command(context, draw_background);
    }

    // Grid
    {
        for (int32 chunk_x = -20; chunk_x <= 20; chunk_x++)
        {
            for (int32 chunk_y = -10; chunk_y <= 10; chunk_y++)
            {
                world_chunk **slot = get_world_chunk_slot(gs, w, chunk_x, chunk_y);
                bool32 have_entities = (slot && (*slot) && (*slot)->entity_count > 0);
                draw_aligned_rectangle(context, gs,
                    (float32)chunk_x * w->chunk_width,
                    (float32)chunk_y * w->chunk_height,
                    w->chunk_width * 0.5f,
                    w->chunk_height * 0.5f,
                    have_entities ? V4(0.8f, 1.f, 0.8f, 1.f) : V4(0.8f, 0.8f, 0.8f, 1.f));
            }
        }
    }
    {
        for (int32 x = -21; x < 21; x++)
        {
            float32 x_ = ((float32) x + 0.5f) * w->chunk_width;
            draw_aligned_rectangle(context, gs, x_, 0.f, 0.01f, 6.f, V4(0.5f, 0.5f, 0.5f, 1.0f));
        }
        for (int32 y = -11; y < 11; y++)
        {
            float32 y_ = ((float32) y + 0.5f) * w->chunk_height;
            draw_aligned_rectangle(context, gs, 0.f, y_, 6.f, 0.01f, V4(0.5f, 0.5f, 0.5f, 1.0f));
        }
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

            auto full_distance = 0.f;
            auto direction = normalized(new_p - old_p, &full_distance);
            auto distance = full_distance;
            if (distance == 0) continue;

            collision_data collision = no_collision();

            math::rectangle2 old_aabb = e->aabb;
            math::rectangle2 new_aabb = compute_aabb(e, new_p);

            auto old_min_corner = get_min(old_aabb);
            auto old_max_corner = get_max(old_aabb);

            auto new_min_corner = get_min(new_aabb);
            auto new_max_corner = get_max(new_aabb);

            int32 old_min_chunk_x, old_min_chunk_y;
            get_chunk_coordinates(w, old_min_corner, &old_min_chunk_x, &old_min_chunk_y);

            int32 old_max_chunk_x, old_max_chunk_y;
            get_chunk_coordinates(w, old_max_corner, &old_max_chunk_x, &old_max_chunk_y);

            int32 new_min_chunk_x, new_min_chunk_y;
            get_chunk_coordinates(w, new_min_corner, &new_min_chunk_x, &new_min_chunk_y);

            int32 new_max_chunk_x, new_max_chunk_y;
            get_chunk_coordinates(w, new_max_corner, &new_max_chunk_x, &new_max_chunk_y);

            auto min_chunk_x = min_(old_min_chunk_x, new_min_chunk_x);
            auto min_chunk_y = min_(old_min_chunk_y, new_min_chunk_y);

            auto max_chunk_x = max_(old_max_chunk_x, new_max_chunk_x);
            auto max_chunk_y = max_(old_max_chunk_y, new_max_chunk_y);

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
                                        temp_collision.point = c2;
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
                new_p = old_p + collision.t_in_meters * direction;
                if (collision.entity1->type == ENTITY_CIRCLE && collision.entity2->type == ENTITY_CIRCLE)
                {
                    if (length_squared(new_p - collision.entity2->position) < math::square(collision.entity1->radius + collision.entity2->radius))
                    {
                        new_p = collision.entity2->position + collision.normal * (collision.entity1->radius + collision.entity2->radius);
                    }
                }
                else if (collision.entity1->type == ENTITY_CIRCLE && collision.entity2->type == ENTITY_ALIGNED_RECTANGLE)
                {
                    if (length_squared(new_p - collision.point) < math::square(collision.entity1->radius))
                    {
                        new_p = collision.point + collision.normal * collision.entity1->radius;
                    }
                }
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

                auto p1_ = (0.5f * proj_p1_ * collision.normal + tangent_p1);
                auto p2_ = (0.5f * proj_p2_ * collision.normal + tangent_p2);

                collision.entity1->velocity = p1_ / m1;
                collision.entity2->velocity = p2_ / m2;

                collision.entity1->collided = true;
                collision.entity2->collided = true;
            }

            e->position = new_p;

            new_aabb = compute_aabb(e, new_p);
            move_entity_between_chunks(gs, w, entity_index, old_aabb, new_aabb);

            e->aabb = new_aabb;

            auto ddt = dt_ * (distance / full_distance);
            dt_ -= ddt;
            if (dt_ < EPSILON) break;
        }

        switch (e->type)
        {
            case ENTITY_CIRCLE:
            {
                render_command::command_draw_mesh_with_color draw_mesh;
                draw_mesh.mesh_token = gs->rectangle_mesh;
                draw_mesh.shader_token = gs->circle_shader;
                draw_mesh.model =
                    math::translated(V3(e->position.x, e->position.y, 0),
                    math::rotated_z(e->rotation,
                    math::scaled(V3(e->radius, e->radius, 1),
                        math::matrix4::identity())));
                // draw_mesh.color = e->collided ? V4(1.f, 0.f, 0.f, 1.f) : V4(0.1f + 0.01f * entity_index, 0.32f, 0.72f, 1.0f);
                auto c = e->velocity * 100.0f;
                // draw_mesh.color = e->collided ? V4(1.f, 0.f, 0.f, 1.f) : V4(0.3 * length(c), 0.2, length(c) * 0.9, 1.f);
                draw_mesh.color = V4(0.3 * length(c), 0.2, length(c) * 0.6, 1.f);

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

    DEBUG_END_TIME_MEASUREMENT(update_and_render);
}


#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <rs/resource_system.cpp>
#include <execution_context.cpp>

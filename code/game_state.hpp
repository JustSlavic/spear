#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.hpp>
#include <resource_system.hpp>
#include <math/vector2.hpp>

// namespace game {


struct entity_id
{
    uint32 index;
};

enum entity_type
{
    ENTITY_INVALID = 0,
    ENTITY_CIRCLE,
    ENTITY_ALIGNED_RECTANGLE,
    ENTITY_ORIENTED_RECTANGLE,
};

struct entity
{
    entity_type type;

    math::vector2 position;
    math::vector2 velocity;
    union
    {
        float32 radius;
        float32 width;
    };
    float32 height;
    float32 mass;
    float32 rotation;
    float32 rotational_velocity;

    math::rectangle2 aabb;
    bool32 collided;
    bool32 deleted;
};

struct world_chunk
{
    int32 x, y;
    uint32 entities[16];
    uint32 entity_count;
    world_chunk *next;
};

struct world
{
    float32 chunk_width;
    float32 chunk_height;

    world_chunk *free_list;

    world_chunk *hash_table[10240];
};

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

struct game_state
{
    memory::allocator game_allocator;

    world world;

    math::vector3 camera_position;
    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;
    rs::resource_token circle_shader;

    entity *entities;
    usize entities_capacity;
    usize entity_count;

    double energy;
    double energy_last_frame;
};


struct entity_ref
{
    entity *entity;
    uint32 eid;
};


INLINE entity_ref push_entity(game_state *gs)
{
    ASSERT(gs->entity_count < gs->entities_capacity);
    entity_ref result;
    result.eid = (uint32) gs->entity_count;
    result.entity = gs->entities + gs->entity_count++;
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

world_chunk **get_world_chunk_slot(game_state *gs, world *w, int32 chunk_x, int32 chunk_y, bool32 create_slot = false)
{
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
    return slot;
}


void push_entity_in_world_chunk_slot(game_state *gs, world *w, world_chunk **slot, uint32 eid, int32 chunk_x, int32 chunk_y)
{
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
}

void remove_entity_from_world_chunk_slot(game_state *gs, world *w, world_chunk **slot, uint32 eid, int32 chunk_x, int32 chunk_y)
{
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

}


void put_entity_in_chunk(game_state *gs, world *w, uint32 eid)
{
    entity *e = get_entity(gs, eid);

    int32 chunk_x, chunk_y;
    get_chunk_coordinates(w, e->position, &chunk_x, &chunk_y);

    world_chunk **slot = get_world_chunk_slot(gs, w, chunk_x, chunk_y, true);
    if (slot)
    {
        ASSERT(*slot);
        push_entity_in_world_chunk_slot(gs, w, slot, eid, chunk_x, chunk_y);
    }
    else
    {
        ASSERT_FAIL(Not enough slots in the hash table);
    }
}


void move_entity_between_chunks(game_state *gs, world *w, uint32 eid, math::vector2 old_p, math::vector2 new_p)
{
    int32 old_chunk_x, old_chunk_y;
    get_chunk_coordinates(w, old_p, &old_chunk_x, &old_chunk_y);

    int32 new_chunk_x, new_chunk_y;
    get_chunk_coordinates(w, new_p, &new_chunk_x, &new_chunk_y);

    if ((new_chunk_x != old_chunk_x) || (new_chunk_y != old_chunk_y))
    {
        world_chunk **old_slot = get_world_chunk_slot(gs, w, old_chunk_x, old_chunk_y);
        remove_entity_from_world_chunk_slot(gs, w, old_slot, eid, old_chunk_x, old_chunk_y);

        world_chunk **new_slot = get_world_chunk_slot(gs, w, new_chunk_x, new_chunk_y, true);
        if (new_slot)
        {
            ASSERT(*new_slot);
            push_entity_in_world_chunk_slot(gs, w, new_slot, eid, new_chunk_x, new_chunk_y);
        }
        else
        {
            ASSERT_FAIL(Not enough slots in the hash table);
        }
    }
}


// } // namespace game

#endif // GAME_STATE_HPP

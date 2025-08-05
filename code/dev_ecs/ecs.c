#include "ecs.h"

#include <stdlib.h>
#include <string.h>


#define ECS_ENTITY_ID_INDEX_BITS (10)
#define ECS_ENTITY_ID_INDEX_MASK ((1 << ECS_ENTITY_ID_INDEX_BITS) - 1)
#define ECS_ENTITY_ID_GENERATION_BITS (sizeof(entity_id)*8 - ECS_ENTITY_ID_INDEX_BITS)
#define ECS_ENTITY_ID_GENERATION_MASK ((1 << ECS_ENTITY_ID_GENERATION_BITS) - 1)
#define ECS_MAX_ENTITIES (1 << ECS_ENTITY_ID_INDEX_BITS)
#define ECS_MAX_ARCHETYPES (32)

ecs_components ecs_components_create(ecs_component *comps, uint32 count)
{
    ecs_components result;
    result.data = malloc(sizeof(ecs_component) * count);
    result.count = count;

    memcpy(result.data, comps, sizeof(ecs_component) * count);
    return result;
}

entity_id ecs_entity_id_create(uint32 generation, uint32 index)
{
    entity_id result = generation << (ECS_ENTITY_ID_INDEX_BITS) | (index);
    return result;
}

uint32 ecs_entity_id_get_index(entity_id eid)
{
    uint32 result = (eid & ECS_ENTITY_ID_INDEX_MASK);
    return result;
}

uint32 ecs_entity_id_get_generation(entity_id eid)
{
    uint32 result = (eid >> ECS_ENTITY_ID_INDEX_BITS) & ECS_ENTITY_ID_GENERATION_MASK;
    return result;
}

void ecs_init(ecs *ecs)
{
    int i;

    ecs->entities = malloc(sizeof(*(ecs->entities)) * ECS_MAX_ENTITIES);
    ecs->empty_slots = malloc(sizeof(*(ecs->empty_slots)) * ECS_MAX_ENTITIES);
    ecs->archetypes = malloc(sizeof(*(ecs->archetypes)) * ECS_MAX_ARCHETYPES);

    memset(ecs->entities, 0, sizeof(*(ecs->entities)) * ECS_MAX_ENTITIES);
    memset(ecs->empty_slots, 0, sizeof(*(ecs->empty_slots)) * ECS_MAX_ENTITIES);
    memset(ecs->archetypes, 0, sizeof(*(ecs->archetypes)) * ECS_MAX_ARCHETYPES);

    ecs->archetype_capacity = ECS_MAX_ARCHETYPES;
    ecs->archetype_count = 0;
    ecs->read_index = 0;
    ecs->write_index = 0;

    // Start from 1 to reserve INVALID_ENTITY_ID
    for (i = 1; i < ECS_MAX_ENTITIES; i++)
    {
        ecs->empty_slots[ecs->write_index++] = i;
    }
}

void ecs_deinit(ecs *ecs)
{
    free(ecs->entities);
    free(ecs->empty_slots);
}

ecs_entity_info *ecs_entity_info_get(ecs *ecs, entity_id eid)
{
    uint32 index = ecs_entity_id_get_index(eid);
    ecs_entity_info *result = ecs->entities + index;
    return result;
}

uint32 ecs_archetype_create(ecs *ecs, uint32 entity_size)
{
    uint32 archetype_id = INVALID_ARCHETYPE_ID;
    ASSERT_MSG(ecs->archetype_count < ecs->archetype_capacity,
        "EcsError: Reached maximum number of archetypes (%llu)", ecs->archetype_capacity);
    if (ecs->archetype_count < ecs->archetype_capacity)
    {
        archetype_id = ecs->archetype_count;
        ecs_archetype *archetype = ecs->archetypes + archetype_id;
        archetype->entity_size = entity_size;
        archetype->entity_count = 0;

        uint32 size = entity_size * 32;
        archetype->size = size;
        archetype->memory = malloc(size);
        memset(archetype->memory, 0, size);

        ecs->archetype_count += 1;
    }
    return archetype_id;
}

ecs_archetype *ecs_archetype_get(ecs *ecs, uint32 archetype_id)
{
    ecs_archetype *result = NULL;
    if (archetype_id < ecs->archetype_count)
    {
        result = ecs->archetypes + archetype_id;
    }
    return result;
}

uint32 ecs_archetype_push_entity(ecs_archetype *archetype, entity_id eid)
{
    UNUSED(eid);
    if (archetype->entity_size * (archetype->entity_count + 1) > archetype->size)
    {
        uint32 new_size = archetype->size * 2;
        void *new_ptr = realloc(archetype->memory, new_size);
        ASSERT(new_ptr);
        if (new_ptr)
        {
            archetype->memory = new_ptr;
            archetype->size = new_size;
        }
    }

    uint32 result = archetype->entity_count;
    archetype->entity_count += 1;
    return result;
}

entity_id ecs_entity_create(ecs *ecs, uint32 archetype_id)
{
    entity_id eid = INVALID_ENTITY_ID;
    if (ecs->read_index < ecs->write_index)
    {
        ASSERT_MSG(archetype_id < ecs->archetype_count,
            "EcsError: Could not create entity of unknown archetype (archetype_id=%u)", archetype_id);
        if (archetype_id < ecs->archetype_count)
        {
            uint32 index = ecs->empty_slots[(ecs->read_index++) % ECS_MAX_ENTITIES];
            ecs_entity_info *info = ecs->entities + index;
            info->archetype_id = archetype_id;
            eid = ecs_entity_id_create(info->generation, index);

            ecs_archetype *archetype = ecs->archetypes + archetype_id;
            info->index_in_archetype = ecs_archetype_push_entity(archetype, eid);

            printf("Ecs: Created entity (eid=%d (%d:%d))\n", eid, ecs_entity_id_get_generation(eid), ecs_entity_id_get_index(eid));
        }
    }
    else
    {
        fprintf(stderr, "Ecs: Reached maximum number of entities (%d)\n", ECS_MAX_ENTITIES);
    }
    return eid;
}

void ecs_entity_destroy(ecs *ecs, entity_id eid)
{
    if (eid)
    {
        uint32 index = ecs_entity_id_get_index(eid);
        ecs->entities[index].generation += 1;
        ecs->empty_slots[(ecs->write_index++) % ECS_MAX_ENTITIES] = index;

        printf("Ecs: Destroyed entity (eid=%d (%d:%d))\n", eid, ecs_entity_id_get_generation(eid), ecs_entity_id_get_index(eid));
    }
}

bool32 ecs_entity_exists(ecs *ecs, entity_id eid)
{
    ecs_entity_info *info = ecs_entity_info_get(ecs, eid);
    uint32 generation = ecs_entity_id_get_generation(eid);
    bool32 result = (generation == info->generation);
    return result;
}

void *ecs_entity_get(ecs *ecs, entity_id eid)
{
    void *result = NULL;
    ecs_entity_info *info = ecs_entity_info_get(ecs, eid);
    ecs_archetype *archetype = ecs_archetype_get(ecs, info->archetype_id);

    if (info->index_in_archetype < archetype->entity_count)
    {
        result = archetype->memory + info->index_in_archetype * archetype->entity_size;
    }
    return result;
}

ecs_entity_iterator ecs_entity_iterator_create(ecs *ecs, uint32 archetype_id)
{
    ecs_archetype *archetype = ecs_archetype_get(ecs, archetype_id);

    ecs_entity_iterator result;
    result.ptr = archetype->memory;
    result.entity_count = archetype->entity_count;
    result.entity_size = archetype->entity_size;

    return result;
}

void *ecs_entity_iterator_get(ecs_entity_iterator *it)
{
    void *result = NULL;
    if (it->entity_count > 0)
    {
        result = it->ptr;
        it->entity_count -= 1;
        it->ptr += it->entity_size;
    }
    return result;
}

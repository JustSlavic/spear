#include "ecs.h"

#include <stdlib.h>
#include <string.h>

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

    ecs->entity_info = malloc(sizeof(*(ecs->entity_info)) * ECS_MAX_ENTITIES);
    ecs->empty_slots = malloc(sizeof(*(ecs->empty_slots)) * ECS_MAX_ENTITIES);
    ecs->archetypes = malloc(sizeof(*(ecs->archetypes)) * ECS_MAX_ARCHETYPES);

    memset(ecs->entity_info, 0, sizeof(*(ecs->entity_info)) * ECS_MAX_ENTITIES);
    memset(ecs->empty_slots, 0, sizeof(*(ecs->empty_slots)) * ECS_MAX_ENTITIES);
    memset(ecs->archetypes, 0, sizeof(*(ecs->archetypes)) * ECS_MAX_ARCHETYPES);

    ecs->read_index = 0;
    ecs->write_index = 0;
    ecs->archetype_count = 0;
    ecs->archetype_capacity = ECS_MAX_ARCHETYPES;

    // Start from 1 to reserve INVALID_ENTITY_ID
    for (i = 1; i < ECS_MAX_ENTITIES; i++)
    {
        ecs->empty_slots[ecs->write_index++] = i;
    }
}

void ecs_deinit(ecs *ecs)
{
    free(ecs->entity_info);
    free(ecs->empty_slots);
}

archetype_id ecs_archetype_create(ecs *ecs, char const *name, uint32 entity_size)
{
    archetype_id result = INVALID_ARCHETYPE_ID;
    if (ecs->archetype_count < ecs->archetype_capacity)
    {
        ecs_archetype *a = ecs->archetypes + ecs->archetype_count;
        a->name = name;

        uint32 buffer_size = entity_size * ECS_MAX_ENTITIES_PER_ARCHETYPE;
        uint32 eids_size = sizeof(*(a->entity_ids)) * ECS_MAX_ENTITIES_PER_ARCHETYPE;

        a->data = malloc(buffer_size);
        a->entity_ids = malloc(eids_size);
        a->entity_size = entity_size;
        a->count = 0;
        a->capacity = ECS_MAX_ENTITIES_PER_ARCHETYPE;

        memset(a->data, 0, buffer_size);
        memset(a->entity_ids, 0, eids_size);

        result = ecs->archetype_count;
        ecs->archetype_count += 1;
    }
    return result;
}

void ecs_archetype_push_entity(ecs_archetype *archetype, archetype_id aid, ecs_entity_info *info, entity_id eid)
{
    if (archetype->count < archetype->capacity)
    {
        int i;
        for (i = 0; i < archetype->capacity; i++)
        {
            if (archetype->entity_ids[i] == INVALID_ENTITY_ID) { break; }
        }
        if (i == archetype->capacity) { printf("Ecs Error: Something went wrong, archetype count < archetype capacity, but no slot for entity id found.\n"); }

        archetype->entity_ids[i] = eid;

        info->archetype = aid;
        info->index_in_archetype = i;

        void *entity_data = archetype->data + i * archetype->entity_size;
        memset(entity_data, 0, archetype->entity_size);
    }
}

ecs_entity_info *ecs_entity_info_get(ecs *ecs, entity_id eid)
{
    uint32 index = ecs_entity_id_get_index(eid);
    ecs_entity_info *result = ecs->entity_info + index;
    return result;
}

entity_id ecs_entity_create(ecs *ecs, archetype_id aid)
{
    entity_id eid = INVALID_ENTITY_ID;
    if (ecs->read_index < ecs->write_index)
    {
        if (aid < ecs->archetype_count)
        {
            ecs_archetype *archetype = ecs->archetypes + aid;
            if (archetype->count < archetype->capacity)
            {
                uint32 index = ecs->empty_slots[(ecs->read_index++) % ECS_MAX_ENTITIES];
                ecs_entity_info *info = ecs->entity_info + index;
                eid = ecs_entity_id_create(info->generation, index);

                ecs_archetype_push_entity(archetype, aid, info, eid);

                info->archetype = aid;
                info->index_in_archetype = archetype->count;
                archetype->count += 1;
                printf("Ecs: Entity created (eid=%d (%d:%d); aid=%d)\n", eid,
                    ecs_entity_id_get_generation(eid), ecs_entity_id_get_index(eid),
                    aid);
            }
            else
            {
                printf("Ecs Error: Reached maximum number of entities per archetype (%d)\n", ECS_MAX_ENTITIES_PER_ARCHETYPE);
            }
        }
    }
    else
    {
        printf("Ecs Error: Reached maximum number of entities (%d)\n", ECS_MAX_ENTITIES);
    }
    return eid;
}

void ecs_entity_destroy(ecs *ecs, entity_id eid)
{
    if (eid)
    {
        uint32 index = ecs_entity_id_get_index(eid);
        ecs->entity_info[index].generation += 1;
        ecs->empty_slots[(ecs->write_index++) % ECS_MAX_ENTITIES] = index;

        printf("Ecs: Destroyed entity (eid=%d (%d:%d))\n", eid,
            ecs_entity_id_get_generation(eid), ecs_entity_id_get_index(eid));
    }
}

bool32 ecs_entity_exists(ecs *ecs, entity_id eid)
{
    if (eid == INVALID_ENTITY_ID)
        return false;
    ecs_entity_info *info = ecs_entity_info_get(ecs, eid);
    uint32 generation = ecs_entity_id_get_generation(eid);
    bool32 result = (generation == info->generation);
    return result;
}

void *ecs_entity_get(ecs *ecs, entity_id eid)
{
    void *result = NULL;
    ecs_entity_info *info = ecs_entity_info_get(ecs, eid);
    return result;
}

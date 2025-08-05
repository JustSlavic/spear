#include "entity_manager.h"


uint32 entity_id_index_get(entity_id eid)
{
    uint32 result = (eid & ENTITY_ID_INDEX_MASK);
    return result;
}

uint32 entity_id_generation_get(entity_id eid)
{
    uint32 result = (eid >> ENTITY_ID_INDEX_BITS);
    return result;
}

static entity_id entity_id_create(uint32 generation, uint32 index)
{
    entity_id result = generation << (ENTITY_ID_INDEX_BITS) | (index);
    return result;
}

void entity_manager_init(entity_manager *em, memory_allocator allocator)
{
    em->allocator = allocator;
    em->generations = ALLOCATE_BUFFER(allocator, sizeof(*(em->generations)) * MAX_ENTITIES);
    em->empty_slots = ALLOCATE_BUFFER(allocator, sizeof(*(em->empty_slots)) * MAX_ENTITIES);
    em->read_index = 0;
    em->write_index = 0;

    int i; // Start from 1 to reserve INVALID_ENTITY_ID
    for (i = 1; i < MAX_ENTITIES; i++)
    {
        em->empty_slots[em->write_index++] = i;
    }
}

void entity_manager_deinit(entity_manager *em)
{
    DEALLOCATE(em->allocator, em->generations);
    DEALLOCATE(em->allocator, em->empty_slots);
}

entity_id entity_manager_entity_create(entity_manager *em)
{
    entity_id eid = INVALID_ENTITY_ID;
    if (em->read_index < em->write_index)
    {
        uint32 index = em->empty_slots[(em->read_index++) % MAX_ENTITIES];
        eid = entity_id_create(em->generations[index], index);

        printf("Ecs: Created entity (eid=%d (%d:%d))\n",
            eid,
            entity_id_generation_get(eid),
            entity_id_index_get(eid));
    }
    else
    {
        fprintf(stderr, "Ecs: Reached maximum number of entities (%d)\n", MAX_ENTITIES);
    }
    return eid;
}

void entity_manager_entity_destroy(entity_manager *em, entity_id eid)
{
    if (eid)
    {
        uint32 index = entity_id_index_get(eid);
        em->generations[index] += 1;
        em->empty_slots[(em->write_index++) % MAX_ENTITIES] = index;

        printf("Ecs: Destroyed entity (eid=%d (%d:%d))\n",
            eid,
            entity_id_generation_get(eid),
            entity_id_index_get(eid));
    }
}

bool entity_manager_entity_exists(entity_manager *em, entity_id eid)
{
    bool result = false;
    if (eid)
    {
        uint32 index = entity_id_index_get(eid);
        uint32 generation = entity_id_generation_get(eid);
        result = (generation == em->generations[index]);
    }
    return result;
}

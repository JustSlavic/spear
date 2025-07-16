#include "entity_manager.hpp"


namespace ecs {

#define ECS_MAX_ARCHETYPES 32

entity_manager entity_manager::initialize(memory_allocator a)
{
    entity_manager result = {};
    result.allocator = a;
    result.descriptions = ALLOCATE_ARRAY(a, entity_description, ECS_MAX_ENTITIES);
    result.descriptions.resize(ECS_MAX_ENTITIES);
    result.empty_slots = ALLOCATE_ARRAY(a, entity_idx_t, ECS_MAX_ENTITIES);
    result.empty_slots.resize(ECS_MAX_ENTITIES);
    result.archetypes = ALLOCATE_ARRAY(a, archetype, ECS_MAX_ARCHETYPES);

    // Start from 1 to reserve INVALID_ENTITY_ID
    for (int i = 1; i < ECS_MAX_ENTITIES; i++)
    {
        result.empty_slots[result.p_write++] = i;
    }
    return result;
}

entity_id entity_manager::create_entity(archetype_idx_t archetype_index)
{
    entity_id eid = INVALID_ENTITY_ID;
    if (p_read < p_write)
    {
        if (0 <= archetype_index && archetype_index < archetypes.size())
        {
            entity_idx_t entity_index = empty_slots[(p_read++) % ECS_MAX_ENTITIES];
            entity_description *desc = descriptions.data() + entity_index;
            eid = make_entity_id(desc->generation, entity_index);
            archetype *a = archetypes.data() + archetype_index;
            desc->archetype_index = archetype_index;
            desc->index_in_archetype = push_entity(a, allocator, eid);
        }
        else
        {
            ASSERT_FAIL("Error: Archetype (index=%u) does not exist!", archetype_index);
        }
    }
    else
    {
       ASSERT_FAIL("Error: we asked more than ECS_MAX_ENTITIES (=%d) entities!!", ECS_MAX_ENTITIES);
    }

    printf("Created entity (eid=%u (%u|%u))\n", eid.id, eid.get_generation(), eid.get_index());
    return eid;
}

void entity_manager::destroy_entity(entity_id eid)
{
    if (eid == INVALID_ENTITY_ID) return;
    entity_idx_t slot = eid.get_index();
    descriptions[slot].generation += 1;
    empty_slots[(p_write++) % ECS_MAX_ENTITIES] = slot;
}

bool entity_manager::is_entity_exists(entity_id eid)
{
    auto slot = eid.get_index();
    return (descriptions[slot].generation == eid.get_generation());
}

archetype_idx_t make_archetype(entity_manager *em, uint32 entity_size)
{
    archetype a = {};
    a.entity_size = entity_size;
    a.chunk.memory = ALLOCATE_BUFFER(em->allocator, entity_size * ECS_ARCH_CHUNK_MAX_COUNT);
    a.chunk.next = NULL;
    a.chunk.eids.resize(a.chunk.eids.capacity());

    archetype_idx_t result = -1;
    if (em->archetypes.size() < em->archetypes.capacity())
    {
        result = em->archetypes.size();
        em->archetypes.push_back(a);
    }
    else
    {
        ASSERT_FAIL("Not enough space in the archetype array!");
    }
    printf("Created archetype (index=%d)\n", result);
    return result;
}


} // namespace ecs


#include "event.cpp"


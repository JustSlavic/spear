#include "entity_manager.hpp"


namespace ecs {

uint16 get_entity_generation(entity_id eid)
{
    return (eid.id >> ECS_MAX_ENTITIES_POWER_OF_2);
}

uint16 get_entity_slot(entity_id eid)
{
#define ECS_ENTITY_ID_SLOT_MASK ((1 << (ECS_MAX_ENTITIES_POWER_OF_2 + 1)) - 1)
    return (eid.id & ECS_ENTITY_ID_SLOT_MASK);
#undef ECS_ENTITY_ID_SLOT_MASK
}

entity_manager entity_manager::create()
{
    entity_manager result = {};
    for (int i = 0; i < ECS_MAX_ENTITIES; i++)
    {
        result.empty_slots[result.p_write++] = i;
    }
    return result;
}

entity_id entity_manager::create_entity()
{
    entity_id result = {};

    if (p_read == p_write)
    {
       ASSERT_FAIL("Error: we asked more than ECS_MAX_ENTITIES (%d) entities!!", ECS_MAX_ENTITIES);
    }
    else
    {
        uint32 slot = (uint32) empty_slots[(p_read++) % ECS_MAX_ENTITIES];
        uint32 generation = (uint32) generations[slot];
        result.id = (generation << (ECS_MAX_ENTITIES_POWER_OF_2 + 1)) | slot;
    }

    return result;
}

void entity_manager::destroy_entity(entity_id eid)
{
    uint32 slot = get_entity_slot(eid);
    generations[slot] += 1;
    empty_slots[(p_write++) % ECS_MAX_ENTITIES] = slot;
}

bool entity_manager::is_entity_exists(entity_id eid)
{
    auto slot = get_entity_slot(eid);
    return (generations[slot] == get_entity_generation(eid));
}



} // namespace ecs

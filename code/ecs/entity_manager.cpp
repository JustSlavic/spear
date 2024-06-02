#include "entity_manager.hpp"


namespace ecs {

entity_manager entity_manager::create()
{
    entity_manager result = {};
    for (int i = 0; i < ECS_MAX_ENTITIES; i++)
    {
        result.empty_slots[result.p_write++] = i;
    }
    result.create_entity(); // INVALID_ENTITY_ID
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
        entity_idx_t slot = empty_slots[(p_read++) % ECS_MAX_ENTITIES];
        entity_gen_t generation = (uint32) generations[slot];
        result = make_entity_id(generation, slot);
    }

    return result;
}

void entity_manager::destroy_entity(entity_id eid)
{
    entity_idx_t slot = eid.get_index();
    generations[slot] += 1;
    empty_slots[(p_write++) % ECS_MAX_ENTITIES] = slot;
}

bool entity_manager::is_entity_exists(entity_id eid)
{
    auto slot = eid.get_index();
    return (generations[slot] == eid.get_generation());
}


} // namespace ecs


#include "event.cpp"


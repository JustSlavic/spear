#include "archetype.hpp"

#include <memory/allocator.hpp>


namespace ecs {


component make_component(uint32 name_hash, uint32 size, uint32 alignment)
{
    component result;
    result.hash = name_hash;
    result.size = size;
    result.alignment = alignment;
    return result;
}

bool operator == (component lhs, component rhs)
{
    return (lhs.hash == rhs.hash) &&
           (lhs.size == rhs.size);
}

bool operator != (component lhs, component rhs)
{
    return !(lhs == rhs);
}

component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, entity_id value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.value.eid = value;
    return result;
}

component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, bool value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.value.b = value;
    return result;
}

component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, int32 value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.value.i = value;
    return result;
}

component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, float32 value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.value.f = value;
    return result;
}

archetype make_archetype(memory_allocator allocator, component_and_value *comps, uint32 comp_count)
{
    ASSERT(comp_count <= 4);
    archetype result = {};

    uint32 entity_size = 0;
    for (uint32 i = 0; i < comp_count; i++)
    {
        result.comps.push_back(comps[i]);
        entity_size += comps[i].comp.size;
    }
    result.chunk.memory = allocator.allocate_buffer(entity_size * ECS_ARCH_MAX_COUNT);
    result.chunk.eids.resize(result.chunk.eids.capacity());
    return result;
}


void archetype::push_entity(entity_id eid)
{
    int index_in_chunk = 0;
    for (; index_in_chunk < chunk.eids.size(); index_in_chunk++)
    {
        if (chunk.eids[index_in_chunk] == INVALID_ENTITY_ID)
        {
            break;
        }
    }

    usize offset = 0;
    for (int index_in_comps = 0; index_in_comps < comps.size(); index_in_comps++)
    {
        auto *c = &comps[index_in_comps];
        byte *data = chunk.memory.data + offset + index_in_chunk * c->comp.size;

        int bad_deed = index_in_comps == 0 ? 0xedded0ba : 0xaddeadde;
        memcpy(data, &c->value, c->comp.size);
        chunk.eids[index_in_chunk] = eid;
        offset += c->comp.size * chunk.eids.size();
    }
}


} // namespace ecs

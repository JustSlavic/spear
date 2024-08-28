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
    result.eid = value;
    return result;
}

component_and_value make_component_init(uint32 name_hash, uint32 size, uint32 alignment, bool value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.b = value;
    return result;
}

component_and_value make_component_init(uint32 name_hash, uint32 size, uint32 alignment, int32 value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.i = value;
    return result;
}

component_and_value make_component_init(uint32 name_hash, uint32 size, uint32 alignment, float32 value)
{
    component_and_value result;
    result.comp = make_component(name_hash, size, alignment);
    result.f = value;
    return result;
}

archetype make_archetype(component_and_value *comps, uint32 comp_count)
{
    ASSERT(comp_count <= 4);
    archetype result = {};

    uint32 entity_size = 0;
    for (uint32 i = 0; i < comp_count; i++)
    {
        result.comps.push_back(comps[i]);
        entity_size += comps[i].comp.size;
    }
#define ECS_ARCH_MAX_MAGIC 16
    result.chunk = mallocator().allocate_buffer(entity_size * ECS_ARCH_MAX_MAGIC);
    return result;
}

void archetype::allocate(archetype_initializer init)
{


    if (count < capacity)
    {
        byte *init_data = (byte *) init.values.data;
        uint32 skip = 0;
        for (uint32 i = 0; i < comp_count; i++)
        {
            uint32 sub_skip = (comps[i].size * count);
            byte *data = (chunk.data + skip + sub_skip);
            memcpy(data, init_data, comps[i].size);

            skip += (comps[i].size * capacity);
            init_data += comps[i].size;
        }

        count += 1;
    }
}



} // namespace ecs

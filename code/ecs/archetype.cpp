#include "archetype.hpp"

#include <memory/allocator.hpp>


namespace ecs {


archetype make_archetype(component *comps, uint32 comp_count)
{
    ASSERT(comp_count <= 4);
    archetype result;

    uint32 arch_size = 0;
    for (uint32 i = 0; i < comp_count; i++)
    {
        arch_size += comps[i].type_size;
        result.comps[i] = comps[i];
    }
#define ECS_ARCH_MAX_MAGIC 4
    result.count = 0;
    result.capacity = ECS_ARCH_MAX_MAGIC;
    result.comp_count = comp_count;
    result.chunk = mallocator().allocate_buffer(arch_size * ECS_ARCH_MAX_MAGIC);
    return result;
}

uint32 archetype::get_entity_size()
{
    uint32 size = 0;
    for (uint32 i = 0; i < comp_count; i++)
    {
        size += comps[i].type_size;
    }
    return size;
}

uint32 archetype::get_entity_struct_size()
{
    uint32 size = 0;
    for (uint32 i = 0; i < comp_count; i++)
    {
        size += (get_padding((void *) (uint64) size, comps[i].type_alignment) + comps[i].type_size);
    }
    return size;
}

void archetype::allocate(archetype_initializer init)
{
    if (comp_count != init.comp_count) return;

    for (uint32 i = 0; i < comp_count; i++)
    {
        if (comps[i] != init.comps[i]) return;
    }

    if (count < capacity)
    {
        byte *init_data = (byte *) init.values.data;
        uint32 skip = 0;
        for (uint32 i = 0; i < comp_count; i++)
        {
            uint32 sub_skip = (comps[i].type_size * count);
            byte *data = (chunk.data + skip + sub_skip);
            memcpy(data, init_data, comps[i].type_size);

            skip += (comps[i].type_size * capacity);
            init_data += comps[i].type_size;
        }

        count += 1;
    }
}



} // namespace ecs

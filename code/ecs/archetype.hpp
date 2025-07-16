#ifndef ECS_ARCHETYPE_HPP
#define ECS_ARCHETYPE_HPP

#include <base.h>
#include <hash.hpp>
#include <memory/allocator.hpp>


namespace ecs {


typedef int32 archetype_idx_t;

struct archetype
{
    #define ECS_ARCH_CHUNK_MAX_COUNT 32
    struct chunk_t
    {
        memory_buffer memory;
        static_array<entity_id, ECS_ARCH_CHUNK_MAX_COUNT> eids;
        chunk_t *next;
    };

    uint32 entity_size;
    chunk_t chunk;
};

archetype_idx_t push_entity(archetype *a, memory_allocator allocator, entity_id eid);


} // namespace ecs


#endif // ECS_ARCHETYPE_HPP

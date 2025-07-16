#include "archetype.hpp"

#include <memory/allocator.hpp>


namespace ecs {


archetype_idx_t push_entity(archetype *a, memory_allocator allocator, entity_id eid)
{
    archetype_idx_t result = -1;
    for (archetype::chunk_t *chunk = &a->chunk; chunk; chunk = chunk->next)
    {
        for (int index_in_chunk = 0; index_in_chunk < ECS_ARCH_CHUNK_MAX_COUNT; index_in_chunk++)
        {
            if (chunk->eids[index_in_chunk] == INVALID_ENTITY_ID)
            {
                chunk->eids[index_in_chunk] = eid;
                result = index_in_chunk;
                break;
            }
        }
        if (chunk->next == NULL)
        {
            // Allocate new chunk and push it there
        }
    }

    return result;
}


} // namespace ecs

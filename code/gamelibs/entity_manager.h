#ifndef _SPEAR_ECS_H
#define _SPEAR_ECS_H

#include <corelibs/base.h>
#include <corelibs/memory/allocator.h>


typedef uint32 entity_id;
#define INVALID_ENTITY_ID (0)
#define ENTITY_ID_INDEX_BITS (10)
#define ENTITY_ID_INDEX_MASK ((1 << ENTITY_ID_INDEX_BITS) - 1)
#define ENTITY_ID_GENERATION_BITS (sizeof(entity_id)*8 - ENTITY_ID_INDEX_BITS)
#define ENTITY_ID_GENERATION_MASK ((1 << ENTITY_ID_GENERATION_BITS) - 1)
#define MAX_ENTITIES (1 << ENTITY_ID_INDEX_BITS)

uint32 entity_id_index_get(entity_id eid);

typedef struct
{
    memory_allocator allocator;
    uint32 *generations;
    uint32 *empty_slots;
    uint64 read_index;
    uint64 write_index;
} entity_manager;

void entity_manager_init(entity_manager *em, memory_allocator allocator);
void entity_manager_deinit(entity_manager *em);
entity_id entity_manager_entity_create(entity_manager *em);
void entity_manager_entity_destroy(entity_manager *em, entity_id eid);
bool entity_manager_entity_exists(entity_manager *em, entity_id eid);


#endif // _SPEAR_ECS_H


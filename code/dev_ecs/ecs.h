#ifndef _SPEAR_ECS_H
#define _SPEAR_ECS_H

#include <corelibs/base.h>

typedef uint32 archetype_id;
#define INVALID_ARCHETYPE_ID (-1u)
#define INVALID_ARCHETYPE_SLOT_ID (-1u)
#define ECS_MAX_ARCHETYPES (32)
#define ECS_MAX_ENTITIES_PER_ARCHETYPE (32)

typedef uint32 entity_id;
#define INVALID_ENTITY_ID (0)
#define ECS_ENTITY_ID_INDEX_BITS (8)
#define ECS_ENTITY_ID_INDEX_MASK ((1 << ECS_ENTITY_ID_INDEX_BITS) - 1)
#define ECS_ENTITY_ID_GENERATION_BITS (sizeof(entity_id)*8 - ECS_ENTITY_ID_INDEX_BITS)
#define ECS_ENTITY_ID_GENERATION_MASK ((1 << ECS_ENTITY_ID_GENERATION_BITS) - 1)
#define ECS_MAX_ENTITIES (1 << ECS_ENTITY_ID_INDEX_BITS)


typedef struct
{
    uint32 generation;
    archetype_id archetype;
    uint32 index_in_archetype;
} ecs_entity_info;

typedef struct
{
    char const *name;
} ecs_component;

typedef struct
{
    char const *name;

    uint8 *data;
    entity_id *entity_ids;
    uint32 entity_size;
    uint32 count;
    uint32 capacity;
} ecs_archetype;

typedef struct
{
    ecs_entity_info *entity_info;
    uint32 *empty_slots;
    uint64 read_index;
    uint64 write_index;

    ecs_archetype *archetypes;
    uint32 archetype_count;
    uint32 archetype_capacity;
} ecs;

void ecs_init(ecs *ecs);
void ecs_deinit(ecs *ecs);

archetype_id ecs_archetype_create(ecs *ecs, char const *name, uint32 entity_size);

entity_id ecs_entity_create(ecs *ecs, archetype_id arch);
void ecs_entity_destroy(ecs *ecs, entity_id eid);
bool32 ecs_entity_exists(ecs *ecs, entity_id eid);

void *ecs_entity_get(ecs *ecs, entity_id eid);


#endif // _SPEAR_ECS_H

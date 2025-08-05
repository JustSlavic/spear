#ifndef _SPEAR_ECS_H
#define _SPEAR_ECS_H

#include <corelibs/base.h>


typedef uint32 entity_id;
#define INVALID_ENTITY_ID (0)
#define INVALID_ARCHETYPE_ID (-1u)

typedef struct
{
    char const *name;
    uint32 size;
} ecs_component;

typedef struct
{
    ecs_component *data;
    uint32 count;
} ecs_components;

ecs_components ecs_components_create(ecs_component *comps, uint32 count);

typedef struct
{
    void *memory;
    uint32 size;

    uint32 entity_count;
    uint32 entity_size;
} ecs_archetype;

typedef struct
{
    void *ptr;
    uint32 entity_count;
    uint32 entity_size;
} ecs_entity_iterator;

typedef struct
{
    uint32 generation;
    uint32 archetype_id;
    uint32 index_in_archetype;
} ecs_entity_info;

typedef struct
{
    ecs_archetype *archetypes;
    uint64 archetype_capacity;
    uint64 archetype_count;

    ecs_entity_info *entities;
    uint64 *empty_slots;
    uint64 read_index;
    uint64 write_index;
} ecs;

void ecs_init(ecs *ecs);
void ecs_deinit(ecs *ecs);
uint32 ecs_archetype_create(ecs *ecs, uint32 entity_size);
entity_id ecs_entity_create(ecs *ecs, uint32 archetype_id);
void ecs_entity_destroy(ecs *ecs, entity_id eid);
bool32 ecs_entity_exists(ecs *ecs, entity_id eid);
void *ecs_entity_get(ecs *ecs, entity_id eid);
ecs_entity_iterator ecs_entity_iterator_create(ecs *ecs, uint32 archetype_id);
void *ecs_entity_iterator_get(ecs_entity_iterator *it);


#endif // _SPEAR_ECS_H

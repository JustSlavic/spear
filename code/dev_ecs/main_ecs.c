#include <corelibs/base.h>
#include "ecs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    float x, y, z;
} vector;

typedef struct
{
    vector position;
    vector velocity;
    float  health;
} entity;

int main()
{
    ecs ecs;
    ecs_init(&ecs);

    ecs_component comps_[] =
    {
        { .name = "position" },
        { .name = "velocity" },
        { .name = "health" },
    };

    uint32 comps_count = ARRAY_COUNT(comps_);
    uint32 comps_size = comps_count * sizeof(ecs_component);
    ecs_component *comps = malloc(comps_size);
    memcpy(comps, comps_, sizeof(comps_));

    archetype_id soldier_arch_id = ecs_archetype_create(&ecs, "soldier", sizeof(entity));
    archetype_id monster_arch_id = ecs_archetype_create(&ecs, "monster", sizeof(entity));

    entity_id eid0 = ecs_entity_create(&ecs, soldier_arch_id);
    entity_id eid1 = ecs_entity_create(&ecs, soldier_arch_id);
    entity_id eid2 = ecs_entity_create(&ecs, soldier_arch_id);
    ecs_entity_destroy(&ecs, eid1);
    entity_id eid3 = ecs_entity_create(&ecs, monster_arch_id);
    entity_id eid4 = ecs_entity_create(&ecs, monster_arch_id);

    printf("ECS_MAX_ENTITIES = %d\n", ECS_MAX_ENTITIES);
    printf("eid0=%d (exists=%s; pointer=%p)\n", eid0, ecs_entity_exists(&ecs, eid0) ? "true" : "false", ecs_entity_get(&ecs, eid0));
    printf("eid1=%d (exists=%s; pointer=%p)\n", eid1, ecs_entity_exists(&ecs, eid1) ? "true" : "false", ecs_entity_get(&ecs, eid1));
    printf("eid2=%d (exists=%s; pointer=%p)\n", eid2, ecs_entity_exists(&ecs, eid2) ? "true" : "false", ecs_entity_get(&ecs, eid2));
    printf("eid3=%d (exists=%s; pointer=%p)\n", eid3, ecs_entity_exists(&ecs, eid3) ? "true" : "false", ecs_entity_get(&ecs, eid3));
    printf("eid4=%d (exists=%s; pointer=%p)\n", eid4, ecs_entity_exists(&ecs, eid4) ? "true" : "false", ecs_entity_get(&ecs, eid4));

    ecs_deinit(&ecs);
    return 0;
}

#include "ecs.c"

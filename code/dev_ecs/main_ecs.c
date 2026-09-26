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

void print_ecs_slots(ecs *ecs)
{
    int slot_index;
    printf("  [");
    for (slot_index = 0; slot_index < ECS_MAX_ENTITIES; slot_index++)
    {
        printf("%s%d,", (slot_index == (ecs->read_index % ECS_MAX_ENTITIES)) &&
                        (slot_index == (ecs->write_index % ECS_MAX_ENTITIES)) ? "rw" :
                        slot_index == (ecs->read_index % ECS_MAX_ENTITIES) ? "r " :
                        slot_index == (ecs->write_index % ECS_MAX_ENTITIES) ? " w" :
                        "  ",
                        ecs->empty_slots[slot_index]);
    }
    printf("]\n");
}



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

    print_ecs_slots(&ecs);
    entity_id eid0 = ecs_entity_create(&ecs, soldier_arch_id);
    print_ecs_slots(&ecs);
    entity_id eid1 = ecs_entity_create(&ecs, soldier_arch_id);
    print_ecs_slots(&ecs);
    entity_id eid2 = ecs_entity_create(&ecs, soldier_arch_id);
    print_ecs_slots(&ecs);
    ecs_entity_destroy(&ecs, eid1);
    print_ecs_slots(&ecs);
    entity_id eid3 = ecs_entity_create(&ecs, monster_arch_id);
    print_ecs_slots(&ecs);
    entity_id eid4 = ecs_entity_create(&ecs, monster_arch_id);
    print_ecs_slots(&ecs);

    printf("ECS_MAX_ENTITIES = %d\n", ECS_MAX_ENTITIES);
    printf("eid0=%d (exists=%s)\n", eid0, ecs_entity_exists(&ecs, eid0) ? "true" : "false");
    printf("eid1=%d (exists=%s)\n", eid1, ecs_entity_exists(&ecs, eid1) ? "true" : "false");
    printf("eid2=%d (exists=%s)\n", eid2, ecs_entity_exists(&ecs, eid2) ? "true" : "false");
    printf("eid3=%d (exists=%s)\n", eid3, ecs_entity_exists(&ecs, eid3) ? "true" : "false");
    printf("eid4=%d (exists=%s)\n", eid4, ecs_entity_exists(&ecs, eid4) ? "true" : "false");

    ecs_entity_info *info = ecs.entity_info;

    int info_index;
    for (info_index = 0; info_index < ECS_MAX_ENTITIES; info_index++)
    {
        printf("  { gen=%d; arch=%d; idx=%d; }\n",
            info->generation, info->archetype, info->index_in_archetype);
        info += 1;
    }

    ecs_deinit(&ecs);
    return 0;
}

#include "ecs.c"

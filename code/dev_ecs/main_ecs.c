#include <corelibs/base.h>
#include <gamelibs/ecs.h>

#include <stdio.h>


typedef struct entity
{
    float x, y;
} entity;

void create_entity_at(ecs *ecs, uint32 archetype, float x, float y)
{
    entity_id eid = ecs_entity_create(ecs, archetype);
    entity *e = ecs_entity_get(ecs, eid);
    e->x = x;
    e->y = y;
}

int main()
{
    ecs ecs;
    ecs_init(&ecs);

    ecs_component comps_[] =
    {
        { .name = "coord_x", .size = sizeof(float) },
        { .name = "coord_y", .size = sizeof(float) },
    };
    ecs_components comps = ecs_components_create(comps_, ARRAY_COUNT(comps_));
    UNUSED(comps);

    int i;
    for (i = 0; i < comps.count; i++)
    {
        printf("name = %s\n", comps.data[i].name);
    }


    uint32 arch = ecs_archetype_create(&ecs, sizeof(entity));
    create_entity_at(&ecs, arch, 42, 69);
    create_entity_at(&ecs, arch, 2, 690);
    create_entity_at(&ecs, arch, 220, 100);

    ecs_entity_iterator it = ecs_entity_iterator_create(&ecs, arch);
    entity *e;
    while ((e = ecs_entity_iterator_get(&it)))
    {
        printf("e={ x=%f, y=%f };\n", e->x, e->y);
    }

    ecs_deinit(&ecs);
    return 0;
}

#include <corelibs/base.c>
#include <gamelibs/ecs.c>

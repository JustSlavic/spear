#ifndef ECS_ARCHETYPE_HPP
#define ECS_ARCHETYPE_HPP

#include <base.h>
#include "component.hpp"


namespace ecs {


struct archetype_initializer {
    component comps[4];
    uint32 comp_count;
    memory_buffer values;
};

struct archetype
{
    component comps[4];
    uint32 comp_count;
    memory_buffer chunk;
    uint32 count;
    uint32 capacity;

    uint32 get_entity_size();
    uint32 get_entity_struct_size();
    void allocate(archetype_initializer init);
};

archetype make_archetype(component *comps, uint32 comp_count);


} // namespace ecs


#endif // ECS_ARCHETYPE_HPP

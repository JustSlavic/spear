#ifndef ECS_ARCHETYPE_HPP
#define ECS_ARCHETYPE_HPP

#include <base.h>


namespace ecs {

#define ECS_COMPONENT_1(STUB)
#define ECS_COMPONENT_2(NAME, TYPE)        ::ecs::make_component(ECS_HASH(STRINGIFY(TYPE)), sizeof(TYPE), alignof(TYPE))
#define ECS_COMPONENT_3(NAME, TYPE, VALUE) ::ecs::make_component_and_value(ECS_HASH(STRINGIFY(TYPE)), sizeof(TYPE), alignof(TYPE), (VALUE))
#define ECS_COMPONENT(...) MACRO_EXPAND(MACRO_OVERLOAD_3(__VA_ARGS__, ECS_COMPONENT_3, ECS_COMPONENT_2, ECS_COMPONENT_1)(__VA_ARGS__))

struct component
{
    uint32 hash;
    uint32 size;
    uint32 alignment;
};

struct component_and_value
{
    component comp;
    union
    {
        entity_id eid;
        bool b;
        int32 i;
        float32 f;
    };
};

component make_component(uint32 name_hash, uint32 size, uint32 alignment);

bool operator == (component lhs, component rhs);
bool operator != (component lhs, component rhs);

component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, entity_id value);
component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, bool value);
component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, int32 value);
component_and_value make_component_and_value(uint32 name_hash, uint32 size, uint32 alignment, float32 value);

struct archetype
{
    static_array<component_and_value, 32> comps;
    memory_buffer chunk;
    uint32 entity_size;

    void create_entity(component_and_value *comps, uint32 comp_count);
};

archetype make_archetype(component_and_value *comps, uint32 comp_count);


} // namespace ecs


#endif // ECS_ARCHETYPE_HPP

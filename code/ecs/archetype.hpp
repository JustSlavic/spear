#ifndef ECS_ARCHETYPE_HPP
#define ECS_ARCHETYPE_HPP

#include <base.h>
#include <hash.hpp>
#include <memory/allocator.hpp>


namespace ecs {

#define ECS_HASH hash_djb2

#define ECS_COMPONENT_1(STUB)
#define ECS_COMPONENT_2(NAME, TYPE)        ::ecs::make_component(ECS_HASH(NAME), sizeof(TYPE))
#define ECS_COMPONENT_3(NAME, TYPE, VALUE) ::ecs::make_component_and_value(ECS_HASH(NAME), sizeof(TYPE), (VALUE))
#define ECS_COMPONENT(...) MACRO_EXPAND(MACRO_OVERLOAD_3(__VA_ARGS__, ECS_COMPONENT_3, ECS_COMPONENT_2, ECS_COMPONENT_1)(__VA_ARGS__))

struct component
{
    uint32 hash;
    uint32 size;
    uint32 offset_in_chunk;
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
    } value;
};

component make_component(uint32 name_hash, uint32 size);

bool operator == (component lhs, component rhs);
bool operator != (component lhs, component rhs);

component_and_value make_component_and_value(uint32 name_hash, uint32 size, entity_id value);
component_and_value make_component_and_value(uint32 name_hash, uint32 size, bool value);
component_and_value make_component_and_value(uint32 name_hash, uint32 size, int32 value);
component_and_value make_component_and_value(uint32 name_hash, uint32 size, float32 value);

struct archetype
{
    #define ECS_ARCH_MAX_COUNT 32
    struct chunk_t
    {
        memory_buffer memory;
        static_array<entity_id, ECS_ARCH_MAX_COUNT> eids;
        chunk_t *next;
    };

    static_array<component_and_value, 32> comps;
    uint32 entity_size;
    chunk_t chunk;

    component get_component_by_name(char const *name);

    void push_entity(entity_id eid);
};

archetype make_archetype(memory_allocator allocator, component_and_value *comps, uint32 comp_count);


} // namespace ecs


#endif // ECS_ARCHETYPE_HPP

#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP

#include <base.h>


namespace ecs {

typedef uint32 entity_id_t ;
typedef entity_id_t entity_idx_t;
typedef entity_id_t entity_gen_t;
#define ECS_ENTITY_INDEX_BITS (5)
#define ECS_ENTITY_INDEX_MASK ((1 << ECS_ENTITY_INDEX_BITS) - 1)
#define ECS_ENTITY_GENERATION_BITS (sizeof(entity_id_t)*8 - ECS_ENTITY_INDEX_BITS)
#define ECS_ENTITY_GENERATION_MASK ((1 << ECS_ENTITY_GENERATION_BITS) - 1)
#define ECS_MAX_ENTITIES (1 << ECS_ENTITY_INDEX_BITS)

struct entity_id
{
    entity_id_t id;

    explicit operator bool() { return id != 0; }

    entity_idx_t get_index() { return (id & ECS_ENTITY_INDEX_MASK); }
    entity_gen_t get_generation() { return (id >> ECS_ENTITY_INDEX_BITS) & ECS_ENTITY_GENERATION_MASK; }
};

extern entity_id INVALID_ENTITY_ID;

entity_id make_entity_id(entity_gen_t generation, entity_idx_t index)
{
    entity_id result;
    result.id = (generation << ECS_ENTITY_INDEX_BITS) | (index);
    return result;
}

bool operator == (entity_id eid1, entity_id eid2)
{
    return eid1.id == eid2.id;
}

bool operator != (entity_id eid1, entity_id eid2)
{
    return eid1.id != eid2.id;
}

} // namespace ecs


#endif // ECS_ENTITY_HPP

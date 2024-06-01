#ifndef ECS__COMPONENT_HPP
#define ECS__COMPONENT_HPP

#include <base.h>


namespace ecs {


#define ECS_COMPONENT(NAME, TYPE) ::ecs::make_component(NAME, ECS_HASH(STRINGIFY(TYPE)), sizeof(TYPE), alignof(TYPE))

struct component
{
    char const *name;
    uint32 type_hash;
    uint32 type_size;
    uint32 type_alignment;
};

component make_component(char const *name, uint32 type_hash, uint32 type_size, uint32 type_alignment)
{
    component result;
    result.name = name;
    result.type_hash = type_hash;
    result.type_size = type_size;
    result.type_alignment = type_alignment;
    return result;
}

bool operator == (component lhs, component rhs)
{
    return (strcmp(lhs.name, rhs.name) == 0) &&
           (lhs.type_hash == rhs.type_hash)  &&
           (lhs.type_size == rhs.type_size);
}

bool operator != (component lhs, component rhs)
{
    return !(lhs == rhs);
}


} // namespace ecs


#endif // ECS__COMPONENT_HPP

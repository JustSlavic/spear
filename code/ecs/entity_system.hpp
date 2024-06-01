#ifndef ECS__ENTITY_SYSTEM_HPP
#define ECS__ENTITY_SYSTEM_HPP

#include <base.h>


namespace ecs {


struct system
{
    // should here be archetypes to that this system is applied?
    void *fn
};


} // namespace ecs


#endif // ECS__ENTITY_SYSTEM_HPP

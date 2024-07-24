#ifndef ECS__ENTITY_SYSTEM_HPP
#define ECS__ENTITY_SYSTEM_HPP

#include <base.h>


#define ECS_SYSTEM(NAME) void NAME(context *ctx, game_state *gs, input_state *input)
#define ECS_SYSTEM_CALL(NAME) NAME(ctx, gs, input)


namespace ecs {


struct system
{
    // should here be archetypes to that this system is applied?
};


} // namespace ecs


#endif // ECS__ENTITY_SYSTEM_HPP

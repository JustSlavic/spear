#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.hpp>
#include <math/vector2.hpp>


struct entity
{
    math::vector2 position;
    math::vector2 velocity;
};


struct game_state
{
    entity entities[1024];
    usize  entity_count;
};


#endif // GAME_STATE_HPP

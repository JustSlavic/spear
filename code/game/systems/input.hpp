#ifndef GAME_SYSTEMS_INPUT_HPP
#define GAME_SYSTEMS_INPUT_HPP

#include <base.h>
#include "entity.hpp"


namespace game {


ECS_SYSTEM(game_exit)
{
    if (get_release_count(input->keyboard[KB_ESC]))
    {
        if (get_seconds(input->time - gs->exit_press_time) < 1)
        {
            ctx->exit_game();
        }
        else
        {
            gs->exit_press_time = input->time;
        }
    }
}


} // namespace game


#endif // GAME_SYSTEMS_INPUT_HPP

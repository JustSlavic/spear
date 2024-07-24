#ifndef GAME_SYSTEMS_ORDER_HPP
#define GAME_SYSTEMS_ORDER_HPP

#include <base.h>
#include <ecs/entity_system.hpp>

#include "systems/input.hpp"
#include "systems/update.hpp"
#include "systems/render.hpp"


namespace game {


ECS_SYSTEM(process_input)
{
    ECS_SYSTEM_CALL(game_exit);
}

ECS_SYSTEM(update_stage)
{
}

ECS_SYSTEM(render_stage)
{
    ECS_SYSTEM_CALL(find_selection_tile);
    ECS_SYSTEM_CALL(setup_camera);
    ECS_SYSTEM_CALL(render_game_field);
}

ECS_SYSTEM(on_every_frame)
{
    ECS_SYSTEM_CALL(process_input);
    ECS_SYSTEM_CALL(update_stage);
    ECS_SYSTEM_CALL(render_stage);
}


} // namespace game


#endif // GAME_SYSTEMS_ORDER_HPP

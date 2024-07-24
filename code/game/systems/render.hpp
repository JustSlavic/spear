#ifndef GAME_SYSTEMS_RENDER_HPP
#define GAME_SYSTEMS_RENDER_HPP

#include <base.h>
#include <ecs/entity_system.hpp>


namespace game {

ECS_SYSTEM(setup_camera)
{
    ctx->setup_camera(gs->camera.position, gs->camera.forward, gs->camera.up);
}

ECS_SYSTEM(render_game_field)
{
    int X = gs->grid_size_x;
    int Y = gs->grid_size_y;

    for (int i = 0; i < X + 1; i++)
    {
        ctx->render_square(matrix4::translate(0.f, i - 0.5f*X, 0.f) *
                           matrix4::scale(0.5f*Y, 0.1, 1),
            V4(0.3, 0.6, 0.9, 1), SHADER_COLOR);
    }
    for (int i = 0; i < Y + 1; i++)
    {
        ctx->render_square(matrix4::translate(i - 0.5f*Y, 0.f, 0.f) *
                           matrix4::scale(0.1f, 0.5f*X, 1),
            V4(0.3, 0.6, 0.9, 1), SHADER_COLOR);
    }
    ctx->render_square(matrix4::translate(gs->intersect_x, gs->intersect_y, 0.1f) *
                       matrix4::scale(0.1f, 0.1f, 0),
                       V4(1, 0, 0, 1), SHADER_COLOR);
}


} // namespace game

#endif // GAME_SYSTEMS_RENDER_HPP

#include "game.hpp"
#include "game_interface.hpp"

#include <math/float32.h>

#include <gfx/vertex_buffer_layout.hpp>

#include <math/projective_geometry3.hpp>
#include <collision.hpp>

#include <math/rectangle3.hpp>

#include <game/systems_order.hpp>


INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_buffer game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);
    auto arena = memory_allocator::make_arena(game_memory);

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

    gs->camera = game::camera::look_at(V3(0, 0, 20), V3(0, 0, 0), V3(0, 1, 0));
    gs->camera_speed = 2.f;

    gs->grid_size_x = 3;
    gs->grid_size_y = 2;

    // Init ECS
    gs->entity_manager = ecs::entity_manager::create();
}


UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_buffer game_memory, input_state *input)
{
    auto gs = (game_state *) ctx->game_state;
    game::on_every_frame(ctx, gs, input);
}


#if DLL_BUILD
#include <context.cpp>
#include <string_id.cpp>
#include <memory_bucket.cpp>
#include <memory/allocator.cpp>
#include <collision.cpp>
#include <image/png.cpp>
#include <crc.cpp>
#include <ecs/entity_manager.cpp>
#endif // DLL_BUILD

#include <ecs/entity_id.cpp>

#include "game.hpp"
#include "game_interface.hpp"

#include <math/float32.h>

#include <gfx/vertex_buffer_layout.hpp>

#include <math/projective_geometry3.hpp>
#include <collision.hpp>

#include <math/rectangle3.hpp>

#include <game/systems_order.hpp>


#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))


bool game_state::is_coords_valid(int x, int y)
{
    return (-2 <= x && x <= 2) && (-2 <= y && y <= 2);
}

ecs::entity_id game_state::get_map_eid(int x, int y)
{
    ecs::entity_id result = ecs::INVALID_ENTITY_ID;
    if (is_coords_valid(x, y))
    {
        result = map[x + 2][y + 2];
    }
    return result;
}

void game_state::set_map_eid(int x, int y, ecs::entity_id eid)
{
    if (is_coords_valid(x, y))
    {
        map[x + 2][y + 2] = eid;
    }
}

INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_buffer game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);
    auto arena = memory_allocator::make_arena(game_memory);

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

    memset(gs->map, 0, sizeof(ecs::entity_id) * 5 * 5);

    gs->double_click_interval = duration::milliseconds(5);

    gs->camera = game::camera::look_at(V3(0, -15, 20), V3(0, 0, 0), V3(0, 0, 1));
    gs->camera_speed = 2.f;

    gs->turn_no = 1;
    gs->turn_timer_enabled = false;
    gs->seconds_for_turn = duration::seconds(5);

    gs->move_color = V4(0.4, 0.4, 0.8, 1);
    gs->defence_color = V4(0.2, 0.6, 0.2, 1);
    gs->attack_color = V4(0.8, 0.2, 0.2, 1);

    gs->selected_entity_height = 0.8f;
    gs->regular_entity_height = 0.3f;

    // Init ECS
    gs->entity_manager = ecs::entity_manager::create();

    game::spawn_hero(gs, 0, 0);
    game::spawn_monster(gs, -2, 2);
    game::spawn_monster(gs, 2, -1);
    game::spawn_monster(gs, -1, 2);

    game::spawn_stone(gs, -1,  1);
    game::spawn_stone(gs, -1, -1);
    game::spawn_stone(gs,  1,  1);
    game::spawn_stone(gs,  1, -1);
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

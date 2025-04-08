#include "game.hpp"
#include "game_interface.hpp"

#include <math/float32.h>
#include <math/integer.h>

#include <gfx/vertex_buffer_layout.hpp>

#include <math/projective_geometry3.hpp>
#include <collision.hpp>

#include <math/rectangle3.hpp>

#include <ecs/entity_manager.hpp>
#include <ecs/archetype.hpp>
// #include <ecs/system.hpp>
#include <simd/float4.h>

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


game_field create_game_field(memory_allocator a, uint32 width, uint32 height)
{
    game_field result = {};
    result.map = a.allocate_array<ecs::entity_id>(width * height);
    result.width = width;
    result.height = height;
    return result;
}

bool game_field::is_coords_valid(int32 x, int32 y)
{
    return (x < width) && (y < height);
}

ecs::entity_id game_field::get_eid(int32 x, int32 y)
{
    return map[x * width + y];
}

void game_field::set_eid(int32 x, int32 y, ecs::entity_id eid)
{
    map[x * width + y] = eid;
}


void test_es(int32 width, int32 height)
{
    console::print("width = %d; height = %d;\n", width, height);
}


void test_es_gen(ecs::archetype *archetype)
{
    auto width_comp = archetype->get_component_by_name("width");
    auto height_comp = archetype->get_component_by_name("height");
    auto age_comp = archetype->get_component_by_name("age");

    for (uint32 index_in_chunk = 0; index_in_chunk < archetype->chunk.eids.size(); index_in_chunk++)
    {
        if (archetype->chunk.eids[index_in_chunk] == ecs::INVALID_ENTITY_ID)
            continue;
        test_es(*(int32 *) (archetype->chunk.memory.data + width_comp.offset_in_chunk + index_in_chunk * width_comp.size),
                *(int32 *) (archetype->chunk.memory.data + height_comp.offset_in_chunk + index_in_chunk * height_comp.size));
    }
}


void bind_action_to_button(action_set *set, uint32 button_id, uint32 action_id)
{
    if (action_id < ARRAY_COUNT(set->buttons))
    {
        set->buttons[action_id] = button_id;
    }
}

uint32 get_press_count(game_state *gs, input_state *input, uint32 action_id)
{
    uint32 result = 0;
    action_set *set = &gs->player_actions;
    if (action_id < ARRAY_COUNT(set->buttons))
    {
        uint32 button_id = set->buttons[action_id];
        button_state button = input->keyboard_and_mouse.buttons[button_id];
        result = get_press_count(button);
    }
    return result;
}

uint32 get_release_count(game_state *gs, input_state *input, uint32 action_id)
{
    uint32 result = 0;
    action_set *set = &gs->player_actions;
    if (action_id < ARRAY_COUNT(set->buttons))
    {
        uint32 button_id = set->buttons[action_id];
        button_state button = input->keyboard_and_mouse.buttons[button_id];
        result = get_release_count(button);
    }
    return result;
}

uint32 get_hold_count(game_state *gs, input_state *input, uint32 action_id)
{
    uint32 result = 0;
    action_set *set = &gs->player_actions;
    if (action_id < ARRAY_COUNT(set->buttons))
    {
        uint32 button_id = set->buttons[action_id];
        button_state button = input->keyboard_and_mouse.buttons[button_id];
        result = get_hold_count(button);
    }
    return result;
}

INITIALIZE_MEMORY_FUNCTION(context *ctx, memory_buffer game_memory)
{
    ASSERT(sizeof(game_state) < game_memory.size);
    auto arena = memory_allocator::make_arena(game_memory);

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

    bind_action_to_button(&gs->player_actions, Keyboard_Esc, PlayerAction_ExitGame);
    bind_action_to_button(&gs->player_actions, Keyboard_I, PlayerAction_ToggleFreeCamera);

    bind_action_to_button(&gs->player_actions, Keyboard_W, PlayerAction_MoveCameraUp);
    bind_action_to_button(&gs->player_actions, Keyboard_S, PlayerAction_MoveCameraDown);
    bind_action_to_button(&gs->player_actions, Keyboard_A, PlayerAction_MoveCameraLeft);
    bind_action_to_button(&gs->player_actions, Keyboard_D, PlayerAction_MoveCameraRight);

    bind_action_to_button(&gs->player_actions, Keyboard_F, PlayerAction_MoveCameraForward);
    bind_action_to_button(&gs->player_actions, Keyboard_B, PlayerAction_MoveCameraBackward);

    bind_action_to_button(&gs->player_actions, Keyboard_Up, PlayerAction_RotateCameraUp);
    bind_action_to_button(&gs->player_actions, Keyboard_Down, PlayerAction_RotateCameraDown);
    bind_action_to_button(&gs->player_actions, Keyboard_Left, PlayerAction_RotateCameraLeft);
    bind_action_to_button(&gs->player_actions, Keyboard_Right, PlayerAction_RotateCameraRight);
    bind_action_to_button(&gs->player_actions, Keyboard_Q, PlayerAction_RotateCameraRollLeft);
    bind_action_to_button(&gs->player_actions, Keyboard_E, PlayerAction_RotateCameraRollRight);

    bind_action_to_button(&gs->player_actions, Keyboard_P, PlayerAction_SpawnMonster);
    bind_action_to_button(&gs->player_actions, Keyboard_O, PlayerAction_SpawnStone);

    bind_action_to_button(&gs->player_actions, Keyboard_Up, PlayerAction_RotateCameraUp);

    memset(gs->map, 0, sizeof(ecs::entity_id) * 5 * 5);

    gs->field = create_game_field(gs->allocator, 3, 3);

    gs->double_click_interval = duration::milliseconds(5);

    gs->camera = game::camera::look_at(V3(0, -15, 20), V3(0, 0, 0), V3(0, 0, 1));
    gs->camera_speed = 5.f;

    gs->turn_no = 1;
    gs->turn_timer_enabled = false;
    gs->seconds_for_turn = duration::seconds(5);

    gs->move_color = V4(0.4, 0.4, 0.8, 1);
    gs->defence_color = V4(0.2, 0.6, 0.2, 1);
    gs->attack_color = V4(0.8, 0.2, 0.2, 1);

    gs->selected_entity_height = 0.8f;
    gs->regular_entity_height = 0.3f;

    gs->field_render__gap = 0.3f;
    gs->camera_fly_mode = true;

    // Init ECS
    gs->entity_manager = ecs::entity_manager::initialize(mallocator());

    ecs::component_and_value comps[] = {
        // ECS_COMPONENT("eid", ecs::entity_id, ecs::INVALID_ENTITY_ID)
        ECS_COMPONENT("width", int32, 1),
        ECS_COMPONENT("height", int32, 2),
        ECS_COMPONENT("age", bool, true),
    };

    auto base_entity_archetype = ecs::make_archetype(gs->allocator, comps, ARRAY_COUNT(comps));
    base_entity_archetype.push_entity(gs->entity_manager.create_entity());
    base_entity_archetype.push_entity(gs->entity_manager.create_entity());
    base_entity_archetype.push_entity(gs->entity_manager.create_entity());
    base_entity_archetype.push_entity(gs->entity_manager.create_entity());
    base_entity_archetype.push_entity(gs->entity_manager.create_entity());
    base_entity_archetype.push_entity(gs->entity_manager.create_entity());

    test_es_gen(&base_entity_archetype);

    game::spawn_hero(gs, 0, 0);
    game::spawn_monster(gs, -2,  2);
    game::spawn_monster(gs,  2, -1);
    game::spawn_monster(gs, -1,  2);

    game::spawn_stone(gs, -1,  1);
    game::spawn_stone(gs, -1, -1);
    game::spawn_stone(gs,  1,  1);
    game::spawn_stone(gs,  1, -1);

    // spawn_planet(gs, V3(0, 3, 1), V3(0, 0, 0), 0.1f, 5.f);
    // spawn_planet(gs, V3(15, -1, -2), V3(0, 0, 0), 3.6f, 5.f);
    // spawn_planet(gs, V3(-3, 1, 2), V3(0, 0, 0), 1.6f, 5.f);
    // spawn_planet(gs, V3(-7.5, 0, 0), V3(0, 0, 0), 1.0f, 5.f,
    //     quaternion::rotate_x(to_radians(30)));
    // spawn_planet(gs, V3(-5.0, 0, 0), V3(0, 0, 0), 1.0f, 5.f,
    //     quaternion::rotate_x(to_radians(60)));
    // spawn_planet(gs, V3(-2.5, 0, 0), V3(0, 0, 0), 1.0f, 5.f,
    //     quaternion::rotate_x(to_radians(90)));
    spawn_planet(gs, V3( 0.0, 0, 0), V3(0.1, 0.1, 0), 1.0f, 5.f,
        quaternion::identity());
    // spawn_planet(gs, V3( 2.5, 0, 0), V3(0, 0, 0), 1.0f, 5.f,
    //     quaternion::rotate_x(to_radians(150)));
    // spawn_planet(gs, V3( 5.0, 0, 0), V3(0, 0, 0), 1.0f, 5.f,
    //     quaternion::rotate_x(to_radians(180)));
}


UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_buffer game_memory, input_state *input)
{
    auto gs = (game_state *) ctx->game_state;
    phys::update_world(&gs->phys_world, input->dt);
    game::on_every_frame(ctx, gs, input);
}

namespace phys {

void update_world(phys::world *world, float32 dt)
{
    dt += world->residual_dt;
    int32 num_steps = (int32) floor(dt / PHYS_SIMULATION_FREQUENCY);
    world->residual_dt = dt - num_steps * PHYS_SIMULATION_FREQUENCY;
    for (int step = 0; step < num_steps; step++)
    {
        for (int body_index = 0; body_index < world->body_count; body_index++)
        {
            body *b = world->bodies + body_index;
            b->position = b->position + b->velocity * PHYS_SIMULATION_FREQUENCY;
        }
    }
    console::print("num steps = %d\n", num_steps);
}

} // phys

#if DLL_BUILD
#include <context.cpp>
#include <string_id.cpp>
#include <memory_bucket.cpp>
#include <memory/allocator.cpp>
#include <collision.cpp>
#include <ecs/entity_manager.cpp>
#include <ecs/archetype.cpp>
#endif // DLL_BUILD

#include <ecs/entity_id.cpp>

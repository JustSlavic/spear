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


game_field create_game_field(memory_allocator a, uint32 width, uint32 height)
{
    game_field result = {};
    result.map = ALLOCATE_ARRAY(a, ecs::entity_id, width*height);
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
    arena.print_debug();

    auto gs = arena.allocate<game_state>();
    gs->allocator = arena;
    ctx->game_state = gs;

#define PHYS_MAX_BODY_COUNT 32
    gs->phys_world.memory = ALLOCATE_BUFFER(arena, 2 * PHYS_MAX_BODY_COUNT * sizeof(phys::rigid_body) + PHYS_MAX_BODY_COUNT * sizeof(vector3));
    gs->phys_world.Y = (phys::rigid_body *) gs->phys_world.memory.data;
    gs->phys_world.Y0 = (phys::rigid_body *) (gs->phys_world.memory.data + PHYS_MAX_BODY_COUNT * sizeof(phys::rigid_body));
    gs->phys_world.capacity = PHYS_MAX_BODY_COUNT;

    bind_action_to_button(&gs->player_actions, Keyboard_Esc, PlayerAction_ExitGame);

#if DEBUG
    bind_action_to_button(&gs->player_actions, Keyboard_F1, PlayerAction_DebugDraw_Off);
    bind_action_to_button(&gs->player_actions, Keyboard_F2, PlayerAction_DebugDraw_Fps);
#endif

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

    bind_action_to_button(&gs->player_actions, Keyboard_1, PlayerAction_FollowPlanetPrev);
    bind_action_to_button(&gs->player_actions, Keyboard_2, PlayerAction_FollowPlanetNext);
    bind_action_to_button(&gs->player_actions, Keyboard_3, PlayerAction_FollowPlanetStop);

    bind_action_to_button(&gs->player_actions, Keyboard_Up, PlayerAction_RotateCameraUp);

    gs->map.dim_x = 10;
    gs->map.dim_y = 10;
    gs->map.dim_z = 10;
    gs->map.origin_x = 4;
    gs->map.origin_y = 4;
    gs->map.origin_z = 2;
    gs->map.data = ALLOCATE_ARRAY(arena, uint32, gs->map.dim_x * gs->map.dim_y * gs->map.dim_z);
    gs->map.data.resize(gs->map.data.capacity());
    for (uint32 j = 0; j < gs->map.dim_y; j++)
    {
        for (uint32 i = 0; i < gs->map.dim_x; i++)
        {
            gs->map.set(i, j, gs->map.origin_z, GameMapOccupation_Ground);
        }
    }

    gs->double_click_interval = duration::milliseconds(5);

    gs->camera__default_position = V3(0, -14, 16);
    gs->camera__default_direction = V3(0, 0, -1);
    gs->camera__default_up = V3(0, 1, 0);

    gs->camera = game::camera::look_at(
        gs->camera__default_position,
        gs->camera__default_direction,
        gs->camera__default_up);
    gs->camera_speed = 10.f;

    gs->turn_no = 1;
    gs->turn_timer_enabled = false;
    gs->seconds_for_turn = 5.f;

    gs->move_color = V4(0.4, 0.4, 0.8, 1);
    gs->defence_color = V4(0.2, 0.6, 0.2, 1);
    gs->attack_color = V4(0.8, 0.2, 0.2, 1);

    gs->selected_entity_height = 0.8f;
    gs->regular_entity_height = 0.3f;

    gs->camera_fly_mode = true;

    gs->planet_follow_index = -1;
    gs->follow_distance = 2.f;
    gs->min_follow_distance = 2.f;

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

    game::spawn_hero(gs, 0, 0, 3);
    game::spawn_monster(gs, 2, 2, 3);
    game::spawn_monster(gs, 2, 1, 4);
    game::spawn_monster(gs, 1, 2, 5);

    // game::spawn_stone(gs, -1,  1);
    // game::spawn_stone(gs, -1, -1);
    // game::spawn_stone(gs,  1,  1);
    // game::spawn_stone(gs,  1, -1);

    // spawn_planet(gs, V3(0),               V3(0.f, 0.f, 0.f),    10.0f,  10000.f, V3(0.8f, 0.8f, 0.2f));
    // spawn_planet(gs, V3(50.f, 0.f, 0.f),  V3(0.f, sqrtf(0.1f * 10000.f / 50.f), 0.f), 0.5f,  1.f,    V3(0.2f, 0.4f, 0.7f));
    // spawn_planet(gs, V3(-300.f, 0.f, 0.f),
    //     V3(0.1f, -sqrtf(0.1f * 10000.f / 300.f), 0.f) + V3(0.f, -sqrtf(0.1f * 10.f / 10.f), -0.1f),
    //     0.5f, 1.f, V3(0.2f, 0.2f, 0.2f));
    // spawn_planet(gs, V3(-310.f, 0.f, 0.f),
    //     V3(0.f, -sqrtf(0.1f * 10000.f / 310.f), 0.f),
    //     2.f, 10.f, V3(0.2f, 0.7f, 0.4f));
    // spawn_planet(gs, V3(510.f, 34.f, -1.f),
    //     V3(0.f, sqrtf(0.1f * 10000.f / 510.f), 1.f),
    //     2.f, 10.f, V3(0.3f, 0.4f, 0.6f));
    // spawn_planet(gs, V3(610.f, 300.f, 0.f),
    //     V3(3.f, sqrtf(0.1f * 10000.f / 610.f), -1.f),
    //     2.f, 10.f, V3(0.2f, 0.2f, 0.8f));
    // sqrt(GM/r)

    // spawn_planet(gs, position, V3(10.0f, 0.0f, 10.0f), 1.0f, 0.1f, V3(1));
    // spawn_planet(gs, position + V3(1, 0, 0), velocity, 0.1f, 5.f, orientation, V3(1, 0, 0));
    // spawn_planet(gs, position + V3(0, 1, 0), velocity, 0.1f, 5.f, orientation, V3(0, 1, 0));
    // spawn_planet(gs, position + V3(0, 0, 1), velocity, 0.1f, 5.f, orientation, V3(0, 0, 1));

    // quaternion qq;
    // qq._1 = 3.f; qq.i = 1.f; qq.j = 5.f; qq.k = -2.f;
    // float4 vq = v_set4(3.f, 1.f, 5.f, -2.f);

    // quaternion qq_ans = qq * qq;
    // float4 vq_ans = q_mul(vq, vq);

    // alignas(16) float32 vq_ans_arr[4];
    // v_st(vq_ans_arr, vq_ans);

    // printf("qq = (%f, %f, %f, %f)\n", qq_ans._1, qq_ans.i, qq_ans.j, qq_ans.k);
    // printf("vq = (%f, %f, %f, %f)\n", vq_ans_arr[0], vq_ans_arr[1], vq_ans_arr[2], vq_ans_arr[3]);
}


UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_buffer game_memory, input_state *input)
{
    auto gs = (game_state *) ctx->game_state;
    // if (get_press_count(input->keyboard_and_mouse.buttons[Keyboard_H]))
    {
        phys::update_world(&gs->phys_world, input->dt);
    }
    game::on_every_frame(ctx, gs, input);
}



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

#include <phys/world.cpp>
#include <phys/ode_solver.cpp>

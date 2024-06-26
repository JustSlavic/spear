#include "game.hpp"
#include "game_interface.hpp"

#include <math/float32.h>

#include <gfx/vertex_buffer_layout.hpp>

#include <math/projective_geometry3.hpp>
#include <collision.hpp>

#include <math/rectangle3.hpp>

#include <game/systems.hpp>


#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))


#define TO_INDICES(X, Y) ((X) + 2) ][ ((Y) + 2)
#define TO_COORDS(I, J) ((I) - 2) ][ ((J) - 2)


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

void draw_health_bar(context *ctx, entity *e, float32 x, float32 y, float32 z)
{
    // Render hp
    {
        int hpBarWidth = 10;
        int gap = 2; // px

        // |hp|
        // |hp| gap |hp|
        // |hp| gap |hp| gap |hp|
        // |hp| gap |hp| gap |hp| gap |hp|

        // 0
        // hp_width / 2 + gap / 2
        // 2 hp_width / 2 + 2 gap / 2
        // 3 hp_width / 2 + 3 gap / 2

        float32 startP = -(hpBarWidth / 2 + gap / 2) * (e->max_hp - 1);
        for (int i = 0; i < e->max_hp; i++)
        {
            auto color = i < e->hp ? V4(1, 0.2, 0.1, 1) : V4(0.3, 0.3, 0.3, 1);
            ctx->render_banner(V3(x, y, z + 1),
                matrix4::translate(startP + i * (hpBarWidth + gap), 0, 0) *
                matrix4::scale(hpBarWidth / 2, hpBarWidth / 2, 1)
                , color);
        }
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

    gs->camera = game::camera::look_at(V3(0, -15, 20), V3(0, 0, 0), V3(0, 0, 1));
    gs->camera_speed = 2.f;

    gs->turn_no = 1;
    gs->turn_timer_enabled = false;
    gs->seconds_for_turn = duration::seconds(5);

    // Init ECS
    gs->entity_manager = ecs::entity_manager::create();

    game::spawn_hero(gs, 0, 0);
    game::spawn_monster(gs, -2, 2);
    game::spawn_monster(gs, 2, -1);

    game::spawn_stone(gs, -2,  1);
    game::spawn_stone(gs, -1,  1);
    game::spawn_stone(gs,  1,  0);
    game::spawn_stone(gs,  2, -2);
}

enum a_star_move {
    A_NULL = 0, A_ML, A_MU, A_MD, A_MR,
    P_NULL,     P_ML, P_MU, P_MD, P_MR,
};


bool a_star(context *ctx, game_state *gs,
            int x0, int y0, int x1, int y1,
            a_star_move *result, int result_size,
            bool32 draw = false)
{
    float32 g_cost[5][5] = {};
    float32 h_cost[5][5] = {};
    float32 f_cost[5][5] = {};
    bool32 visited[5][5] = {};
    a_star_move moves[5][5] = {};

    int i0 = x0 + 2;
    int j0 = y0 + 2;

    int i1 = x1 + 2;
    int j1 = y1 + 2;

    for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
    {
        g_cost[i][j] = infinity;
        h_cost[i][j] = absolute(i - i1) + absolute(j - j1);
        f_cost[i][j] = infinity;
    }

    int shifts[4][2] = {
        {  1,  0 },
        { -1,  0 },
        {  0,  1 },
        {  0, -1 },
    };
    int shiftm[4] = {
        A_MR,
        A_ML,
        A_MU,
        A_MD,
    };

    g_cost[i0][j0] = 0.f;
    f_cost[i0][j0] = h_cost[i0][j0];

    while (true)
    {
        // Stop the loop when target node is reached
        if (f_cost[i1][j1] < infinity)
            break;

        int min_i = 0;
        int min_j = 0;
        float32 min_cost = infinity;

        // First, find the minimum of f_cost amongst all tiles
        for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
        {
            if (visited[i][j]) continue;
            if (f_cost[i][j] < min_cost)
            {
                min_i = i;
                min_j = j;
                min_cost = f_cost[i][j];
            }
        }

        // Since we set f_cost[i0][j0] < infinity, the condition is redundant
        if (min_cost < infinity)
        {
            visited[min_i][min_j] = true;
            for (int shift_index = 0; shift_index < 4; shift_index++)
            {
                int shift_x = shifts[shift_index][0];
                int shift_y = shifts[shift_index][1];

                int i = min_i + shift_x;
                int j = min_j + shift_y;

                int x = i - 2;
                int y = j - 2;

                if ((game::cell_is_empty(gs, x, y) ||
                    (i == i1 && j == j1)))
                {
                    float32 new_cost = g_cost[min_i][min_j] + 1.f;
                    if (new_cost < g_cost[i][j])
                    {
                        g_cost[i][j] = new_cost;
                        f_cost[i][j] = new_cost + h_cost[i][j];
                        visited[i][j] = false;
                        if (i < min_i) moves[i][j] = A_MR;
                        if (i > min_i) moves[i][j] = A_ML;
                        if (j < min_j) moves[i][j] = A_MU;
                        if (j > min_j) moves[i][j] = A_MD;
                    }
                }
            }
        }
        else
        {
            return false;
        }
    }

    {
        int i = i1;
        int j = j1;
        auto mv = moves[i][j];
        moves[i][j] = P_NULL;

        while (i != i0 || j != j0)
        {
            if (mv == A_MR)
            {
                // [->][mv?]
                mv = moves[i + 1][j];
                moves[i + 1][j] = P_ML;
                i = i + 1;
            }
            else if (mv == A_ML)
            {
                // [mv?][<-]
                mv = moves[i - 1][j];
                moves[i - 1][j] = P_MR;
                i = i - 1;
            }
            else if (mv == A_MU)
            {
                // [mv?]
                // [^^^]
                mv = moves[i][j + 1];
                moves[i][j + 1] = P_MD;
                j = j + 1;
            }
            else if (mv == A_MD)
            {
                // [vvv]
                // [mv?]
                mv = moves[i][j - 1];
                moves[i][j - 1] = P_MU;
                j = j - 1;
            }
            else
            {
                ASSERT_FAIL();
            }
        }
    }

    {
        int i = i0;
        int j = j0;
        while (result_size > 0)
        {
            *result = moves[i][j];
            result += 1;
            result_size -= 1;
            if (moves[i][j] == P_ML) i -= 1;
            if (moves[i][j] == P_MR) i += 1;
            if (moves[i][j] == P_MU) j += 1;
            if (moves[i][j] == P_MD) j -= 1;
        }
    }

    // Render pathfinding debug
    if (draw)
    {
        for (int x = 0; x < 5; x++) for (int y = 0; y < 5; y++)
        {
            float32 cost = clamp(f_cost[x][y], 0.f, 25.f);
            cost = cvt(cost, 0.f, 25.f, 0.f, 1.f);
            auto color = V4(cost, 1.f - cost, 0.f, 1.f);
            if (moves[x][y] >= P_NULL) color = V4(0.4, 0.4, 0.8, 1);

            ctx->render_ui(
                           matrix4::translate((float32) x * 25.f + 600.f,
                                              (float32) (5 - y) * 25.f + 10.f, 0.f) *
                           matrix4::scale(10, 10, 1),
                           color);
        }
    }

    return true;
}



UPDATE_AND_RENDER_FUNCTION(context *ctx, memory_buffer game_memory, input_state *input)
{
    auto gs = (game_state *) ctx->game_state;
    auto dt = input->dt;

    game::on_every_frame(ctx, gs, input);

    if (get_press_count(input->keyboard[KB_I]))
    {
        TOGGLE(gs->camera_fly_mode);
    }

    if (get_press_count(input->keyboard[KB_G]))
    {
        gs->world_view = gs->world_view == WORLD_VIEW__NORMAL ? WORLD_VIEW__GHOST :
                         gs->world_view == WORLD_VIEW__GHOST ? WORLD_VIEW__NORMAL : WORLD_VIEW__NORMAL;
    }

    if (gs->camera_fly_mode) game::move_camera(gs, input);
    ctx->setup_camera(gs->camera.position, gs->camera.forward, gs->camera.up);

    vector3 ray_direction = game::compute_pointer_ray(ctx, gs, input);
    vector3 intersection;
    {
        auto Oxy = make_plane3(0, 0, 1, 0);
        auto line = make_line3(ray_direction, gs->camera.position);
        auto intersection_p = outer(Oxy, line);
        intersection = intersection_p.vector / intersection_p.w;
    }

    bool32 intersected = false;
    float32 intersect_t = infinity;
    int intersect_x = 0, intersect_y = 0;
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto center = V3(x + 1.3f*x, y + 1.3f*y, 0);
            rectangle3 aabb = rectangle3::from_center_radius(center, 1, 1, 1);

            float tmin;
            int intersect_cube = intersect_ray_aabb(gs->camera.position, ray_direction, aabb, &tmin);

            if (intersect_cube && tmin < intersect_t)
            {
                intersected = true;
                intersect_t = tmin;
                intersect_x = x;
                intersect_y = y;
            }
        }
    }

    entity *selected_entity = game::get_entity(gs, gs->selected_entity_eid);
    entity *hero = game::get_entity(gs, gs->hero_eid);

    bool force_new_turn = get_press_count(input->keyboard[KB_SPACE]);
    bool timer_new_turn = gs->turn_timer_enabled && (input->time >= (gs->turn_start_time + gs->seconds_for_turn));

    if (force_new_turn || timer_new_turn)
    {
        // @attention NEW TURN !!!
        if (gs->hero_eid == ecs::INVALID_ENTITY_ID)
        {
            if (gs->spawn_hero_next_turn)
            {
                if (game::cell_is_empty(gs, 0, 0))
                    game::spawn_hero(gs, 0, 0);
            }
            else
            {
                gs->spawn_hero_next_turn = true;
            }
        }

        gs->action_buffer.clear();
        if (hero)
            gs->action_buffer.push_back(game::get_action2(hero));

        for (int i = 0; i < gs->monsters.size(); i++)
        {
            entity *monster = game::get_entity(gs, gs->monsters[i]);
            gs->action_buffer.push_back(game::get_action2(monster));
        }

        // Cancel intersecting moves
        for (int i = 0; i < gs->action_buffer.size(); i++)
        {
            auto *a1 = &gs->action_buffer[i];
            for (int j = i + 1; j < gs->action_buffer.size(); j++)
            {
                auto *a2 = &gs->action_buffer[j];
                if (a1->kind == ENTITY_ACTION2_MOVE && a2->kind == ENTITY_ACTION2_MOVE)
                {
                    if (a1->x1 == a2->x1 && a1->y1 == a2->y1)
                    {
                        // a1 -> <- a2
                        a1->kind = ENTITY_ACTION2_NONE;
                        a2->kind = ENTITY_ACTION2_NONE;
                    }
                    else if (a1->x1 == a2->x0 && a1->y1 == a2->y0)
                    {
                        // a1 -> a2 ->
                        ASSERT_FAIL("Should not be possible");
                    }
                    else if (a1->x0 == a2->x1 && a1->y0 == a2->y1)
                    {
                        // <- a1 <- a2
                        ASSERT_FAIL("Should not be possible");
                    }
                }
            }
        }

        // Remove null actions
        {
            int i = (int) gs->action_buffer.size();
            while (i-->0)
            {
                if (gs->action_buffer[i].kind == ENTITY_ACTION2_NONE)
                {
                    entity *e = game::get_entity(gs, gs->action_buffer[i].eid);
                    e->action = game::null_action();

                    gs->action_buffer.erase_not_sorted(i);
                }
            }
        }

        for (int i = 0; i < gs->action_buffer.size(); i++)
        {
            auto *a1 = &gs->action_buffer[i];
            for (int j = i + 1; j < gs->action_buffer.size(); j++)
            {
                auto *a2 = &gs->action_buffer[j];
                if (a1->kind == ENTITY_ACTION2_MOVE && a2->kind == ENTITY_ACTION2_ATTACK)
                {
                    if (a1->x0 == a2->x1 && a1->y0 == a2->y1)
                    {
                        // a2 --a-> a1 ->
                        auto tmp = *a1;
                        *a1 = *a2;
                        *a2 = tmp;
                    }
                    else if (a1->x1 == a2->x1 && a1->y1 == a2->y1)
                    {
                        // a2 --a->  <- a1
                        // All right
                    }
                }
                else if (a1->kind == ENTITY_ACTION2_ATTACK && a2->kind == ENTITY_ACTION2_MOVE)
                {
                    if (a1->x0 == a2->x1 && a1->y0 == a2->y1)
                    {
                        // a2 --a-> a1 ->
                        // All right
                    }
                    else if (a1->x1 == a2->x1 && a1->y1 == a2->y1)
                    {
                        // a2 --a->  <- a1
                        auto tmp = *a1;
                        *a1 = *a2;
                        *a2 = tmp;
                    }
                }
                else if (a1->kind == ENTITY_ACTION2_ATTACK && a2->kind == ENTITY_ACTION2_DEFENCE)
                {
                    if ((a1->x1 == a2->x0 && a1->y1 == a2->y0) &&
                        (a1->x0 == a2->x1 && a1->y0 == a2->y1))
                    {
                        // a1 --a-> a2
                        //    <-d--
                        auto tmp = *a1;
                        *a1 = *a2;
                        *a2 = tmp;
                    }
                }
            }
        }

        game::apply_actions(gs);
        game::remove_dead_entities(gs);
        game::reset_entity_states(gs);

        for (auto monster_eid : gs->monsters)
        {
            auto *monster = game::get_entity(gs, monster_eid);
            a_star_move moves[25] = {};
            bool32 path_exists = a_star(ctx, gs, monster->x, monster->y, hero->x, hero->y, moves, ARRAY_COUNT(moves), true);
            if (path_exists)
            {
                int dx = 0;
                int dy = 0;

                if (moves[0] == P_ML) dx = -1;
                if (moves[0] == P_MR) dx = 1;
                if (moves[0] == P_MU) dy = 1;
                if (moves[0] == P_MD) dy = -1;

                for (auto m : moves) {
                    if (m == A_NULL) console::print("A_NULL ");
                    if (m == A_ML) console::print("A_ML ");
                    if (m == A_MR) console::print("A_MR ");
                    if (m == A_MU) console::print("A_MU ");
                    if (m == A_MD) console::print("A_MD ");
                    if (m == P_NULL) console::print("P_NULL ");
                    if (m == P_ML) console::print("P_ML ");
                    if (m == P_MR) console::print("P_MR ");
                    if (m == P_MU) console::print("P_MU ");
                    if (m == P_MD) console::print("P_MD ");
                }
                console::print("\n");

                int x = monster->x + dx;
                int y = monster->y + dy;

                entity_action2 act;
                act.eid = monster->eid;
                act.x0 = monster->x;
                act.y0 = monster->y;
                act.x1 = x;
                act.y1 = y;

                if (x == hero->x && y == hero->y)
                {
                    act.kind = ENTITY_ACTION2_ATTACK;
                    monster->action.kind = ENTITY_ACTION_RIGHT_ARM;
                }
                else
                {
                    act.kind = ENTITY_ACTION2_MOVE;
                    monster->action.kind = ENTITY_ACTION_MOVE;
                }
                monster->action.x = x;
                monster->action.y = y;
                gs->action_buffer.push_back(act);
            }
        }

        gs->turn_no += 1;
        gs->turn_start_time = input->time;
    }

    if (get_press_count(input->keyboard[KB_H]))
    {
        if (hero)
        {
            hero->hp = clamp(hero->hp + 1, 0, hero->max_hp);
        }
    }

    if (intersected && gs->get_map_eid(intersect_x, intersect_y) != ecs::INVALID_ENTITY_ID &&
        !game::entity_can_walk_here(gs, selected_entity, intersect_x, intersect_y) &&
        !gs->selecting_direction_of_action)
    {
        if (get_press_count(input->mouse[MOUSE_LEFT]))
        {
            gs->selected_entity_eid = gs->get_map_eid(intersect_x, intersect_y);
            selected_entity = game::get_entity(gs, gs->selected_entity_eid);
        }
    }

    if (intersected)
    {
        if (game::cell_is_empty(gs, intersect_x, intersect_y))
        {
            if (get_press_count(input->keyboard[KB_P]))
            {
                game::spawn_monster(gs, intersect_x, intersect_y);
            }
            else if (get_press_count(input->keyboard[KB_O]))
            {
                game::spawn_stone(gs, intersect_x, intersect_y);
            }
        }
    }

    if (selected_entity)
    {
        int move_to_x = selected_entity->x;
        int move_to_y = selected_entity->y;

        if (intersected && get_press_count(input->mouse[MOUSE_LEFT]))
        {
            move_to_x = intersect_x;
            move_to_y = intersect_y;
        }
        if (get_press_count(input->keyboard[KB_W]))
        {
            move_to_x = selected_entity->x;
            move_to_y = selected_entity->y + 1;
        }
        if (get_press_count(input->keyboard[KB_A]))
        {
            move_to_x = selected_entity->x - 1;
            move_to_y = selected_entity->y;
        }
        if (get_press_count(input->keyboard[KB_S]))
        {
            move_to_x = selected_entity->x;
            move_to_y = selected_entity->y - 1;
        }
        if (get_press_count(input->keyboard[KB_D]))
        {
            move_to_x = selected_entity->x + 1;
            move_to_y = selected_entity->y;
        }
        if (get_press_count(input->keyboard[KB_Q]))
        {
            gs->action_input.kind = ENTITY_ACTION_LEFT_ARM;
            TOGGLE(gs->selecting_direction_of_action);
        }

        if (get_press_count(input->keyboard[KB_E]))
        {
            gs->action_input.kind = ENTITY_ACTION_RIGHT_ARM;
            TOGGLE(gs->selecting_direction_of_action);
        }

        if (gs->is_coords_valid(move_to_x, move_to_y) &&
            game::cell_is_adjacent_to_entity(selected_entity, move_to_x, move_to_y))
        {
            gs->action_input.x = move_to_x;
            gs->action_input.y = move_to_y;
            if (!gs->selecting_direction_of_action)
            {
                gs->action_input.kind = ENTITY_ACTION_MOVE;
            }

            if (gs->action_input.kind == ENTITY_ACTION_MOVE)
            {
                if (game::entity_can_walk_here(gs, selected_entity, gs->action_input.x, gs->action_input.y))
                {
                    selected_entity->action = gs->action_input;
                    gs->selecting_direction_of_action = false;
                }
            }
            else if (gs->action_input.kind != ENTITY_ACTION_NONE)
            {
                selected_entity->action = gs->action_input;
                gs->selecting_direction_of_action = false;
            }
        }
    }

    // A* pathfinding
    a_star_move moves[25] = {};
    if (intersected && hero)
    {
        a_star(ctx, gs, hero->x, hero->y, intersect_x, intersect_y, moves, ARRAY_COUNT(moves));
    }

    auto move_color = V4(0.4, 0.4, 0.8, 1);
    auto defence_color = V4(0.2, 0.6, 0.2, 1);
    auto attack_color = V4(0.8, 0.2, 0.2, 1);

    // Render ground
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            auto c = gs->world_view == WORLD_VIEW__GHOST
                ? V4(0.2 + 0.1f * y, 0.6 + 0.1f * x, 0.7 - 0.1f * x, 1)
                : V4(0.8 - 0.1f * x, 0.5 + 0.1f * y, 0.3 + 0.1f * x, 1);

            if (gs->selecting_direction_of_action)
            {
                if (game::cell_is_adjacent_to_entity(selected_entity, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }
            }
            else if (selected_entity)
            {
                if (game::entity_can_walk_here(gs, selected_entity, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }

                if (x == selected_entity->action.x &&
                    y == selected_entity->action.y)
                {
                    if (selected_entity->action.kind == ENTITY_ACTION_MOVE)
                        c = move_color;
                    else if (selected_entity->action.kind == ENTITY_ACTION_LEFT_ARM)
                        c = defence_color;
                    else if (selected_entity->action.kind == ENTITY_ACTION_RIGHT_ARM)
                        c = attack_color;
                }
            }

            if (intersected && x == intersect_x && y == intersect_y)
            {
                c += V4(0.3, 0.3, 0.3, 1);
            }

            auto m = matrix4::translate(2.3*x, 2.3*y, 0);

            ctx->render_cube(m, c, SHADER_GROUND);
        }
    }

    float selected_entity_height = 0.8f;
    float regular_entity_height = 0.3f;

    // Draw hero
    if (hero)
    {
        float32 x = 2.3f*hero->x;
        float32 y = 2.3f*hero->y;
        float32 z = 2;
        float height = hero->eid == gs->selected_entity_eid ? selected_entity_height
                     : regular_entity_height;
        auto m = matrix4::translate(x, y, z) *
                 matrix4::scale(0.5f, 0.5f, height);

        ctx->render_cube(m, V4(1, 1, 1, 1), SHADER_COLOR);
        draw_health_bar(ctx, hero, x, y, z);
    }

    // Draw monsters
    {
        for (int i = 0; i < gs->monsters.size(); i++)
        {
            entity *monster = game::get_entity(gs, gs->monsters[i]);
            float32 x = monster->x + 1.3f*monster->x;
            float32 y = monster->y + 1.3f*monster->y;
            float32 z = 2;
            float height = monster->eid == gs->selected_entity_eid ? selected_entity_height
                         : regular_entity_height;

            auto m = matrix4::translate(x, y, z) *
                     matrix4::scale(0.5f, 0.5f, height);
            ctx->render_cube(m, V4(0.9, 0.2, 0.7, 1), SHADER_COLOR);
            draw_health_bar(ctx, monster, x, y, z);

            if (monster->action.kind != ENTITY_ACTION_NONE)
            {
                auto color = monster->action.kind == ENTITY_ACTION_MOVE ? move_color :
                             monster->action.kind == ENTITY_ACTION_LEFT_ARM ? defence_color :
                             monster->action.kind == ENTITY_ACTION_RIGHT_ARM ? attack_color :
                             V4(1, 1, 1, 1);
                ctx->render_square(matrix4::translate(monster->action.x - monster->x,
                                                      monster->action.y - monster->y,
                                                      -selected_entity_height) * m,
                    color, SHADER_COLOR);
            }
        }
    }

    // Draw stones
    {
        for (auto stone_eid : gs->stones)
        {
            entity *stone = game::get_entity(gs, stone_eid);
            float32 x = stone->x + 1.3f*stone->x;
            float32 y = stone->y + 1.3f*stone->y;
            float32 z = 2;

            auto m = matrix4::translate(x, y, z);
            ctx->render_cube(m, V4(0.6, 0.8, 0.1, 1), SHADER_COLOR);
        }
    }

    // Draw character page
    if (hero)
    {
        auto color_left_arm = V4(0.2, 0.5, 0.7, 1.0);
        auto color_torso = V4(0.2, 0.5, 0.7, 1.0);
        auto color_right_arm = V4(0.2, 0.5, 0.7, 1.0);

        if ((!gs->selecting_direction_of_action && hero->action.kind == ENTITY_ACTION_LEFT_ARM) ||
            (gs->selecting_direction_of_action && gs->action_input.kind == ENTITY_ACTION_LEFT_ARM))
            color_left_arm += V4(0.5, 0.2, 0.1, 0);
        if ((!gs->selecting_direction_of_action && hero->action.kind == ENTITY_ACTION_RIGHT_ARM) ||
            (gs->selecting_direction_of_action && gs->action_input.kind == ENTITY_ACTION_RIGHT_ARM))
            color_right_arm += V4(0.5, 0.2, 0.1, 0);

        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(25) *
                       matrix4::scale(25, 50, 1)
            , color_left_arm);
        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(80) *
                       matrix4::scale(25, 50, 1)
            , color_torso);
        ctx->render_ui(
                       matrix4::translate_y(50) *
                       matrix4::translate_x(135) *
                       matrix4::scale(25, 50, 1)
            , color_right_arm);
    }

    // Render timer
    if (gs->turn_timer_enabled)
    {
        float32 t = (float32) (1.f - get_seconds(input->time - gs->turn_start_time) / get_seconds(gs->seconds_for_turn));
        vector4 color = V4(sin((t - 3) * pi * 0.5f),
                           -cos((t + 1) * pi * 0.5f),
                           0, 1);
        ctx->render_square(
                       matrix4::translate_y((float32) (ctx->viewport.height - 10)) *
                       matrix4::scale(ctx->viewport.width * t, 2, 1)
            , color, SHADER_COLOR);
    }

    // Render action buffer
    {
        int x = 200;
        int y = 20;
        for (auto action : gs->action_buffer)
        {
            auto color = action.kind == ENTITY_ACTION2_MOVE ? V4(0.4, 0.4, 0.8, 1) :
                         action.kind == ENTITY_ACTION2_ATTACK ? V4(0.8, 0.4, 0.4, 1) :
                         action.kind == ENTITY_ACTION2_DEFENCE ? V4(0.4, 0.8, 0.4, 1) :
                         V4(0.2, 0.2, 0.2, 1);
            ctx->render_ui(
                           matrix4::translate_x((float32) x) *
                           matrix4::translate_y((float32) y) *
                           matrix4::scale(10, 10, 1)
                , color);

            x += 25;
        }
    }
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

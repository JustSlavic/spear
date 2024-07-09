#ifndef GAME_SYSTEMS_RENDER_HPP
#define GAME_SYSTEMS_RENDER_HPP


namespace game {


void render_character_page(context *ctx, game_state *gs, input_state *)
{
    entity *hero = game::get_entity(gs, gs->hero_eid);
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
}

void render_ground(context *ctx, game_state *gs, input_state *)
{
    entity *selected_entity = game::get_entity(gs, gs->hero_eid);
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
            else if (selected_entity && gs->selected_entity_eid == gs->hero_eid)
            {
                if (game::entity_can_walk_here(gs, selected_entity, x, y))
                {
                    c += V4(0.3, 0.3, 0.3, 0);
                }

                if (x == selected_entity->action.x &&
                    y == selected_entity->action.y)
                {
                    if (selected_entity->action.kind == ENTITY_ACTION_MOVE)
                        c = gs->move_color;
                    else if (selected_entity->action.kind == ENTITY_ACTION_LEFT_ARM)
                        c = gs->defence_color;
                    else if (selected_entity->action.kind == ENTITY_ACTION_RIGHT_ARM)
                        c = gs->attack_color;
                }
            }

            if (gs->intersected && x == gs->intersect_x && y == gs->intersect_y)
            {
                c += V4(0.3, 0.3, 0.3, 1);
            }

            auto m = matrix4::translate(2.3*x, 2.3*y, 0);

            ctx->render_cube(m, c, SHADER_GROUND);
        }
    }
}

void render_stones(context *ctx, game_state *gs, input_state *)
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

void render_hero(context *ctx, game_state *gs, input_state *)
{
    entity *hero = game::get_entity(gs, gs->hero_eid);
    if (hero)
    {
        float32 x = 2.3f*hero->x;
        float32 y = 2.3f*hero->y;
        float32 z = 2;
        float height = hero->eid == gs->selected_entity_eid ? gs->selected_entity_height
                     : gs->regular_entity_height;
        auto m = matrix4::translate(x, y, z) *
                 matrix4::scale(0.5f, 0.5f, height);

        ctx->render_cube(m, V4(1, 1, 1, 1), SHADER_COLOR);
        draw_health_bar(ctx, hero, x, y, z);
    }
}

void render_monsters(context *ctx, game_state *gs, input_state *)
{
    for (int i = 0; i < gs->monsters.size(); i++)
    {
        entity *monster = game::get_entity(gs, gs->monsters[i]);
        float32 x = monster->x + 1.3f*monster->x;
        float32 y = monster->y + 1.3f*monster->y;
        float32 z = 2;
        float height = monster->eid == gs->selected_entity_eid ? gs->selected_entity_height
                     : gs->regular_entity_height;

        auto m = matrix4::translate(x, y, z) *
                 matrix4::scale(0.5f, 0.5f, height);
        ctx->render_cube(m, V4(0.9, 0.2, 0.7, 1), SHADER_COLOR);
        draw_health_bar(ctx, monster, x, y, z);

        if (monster->action.kind != ENTITY_ACTION_NONE)
        {
            auto color = monster->action.kind == ENTITY_ACTION_MOVE ? gs->move_color :
                         monster->action.kind == ENTITY_ACTION_LEFT_ARM ? gs->defence_color :
                         monster->action.kind == ENTITY_ACTION_RIGHT_ARM ? gs->attack_color :
                         V4(1, 1, 1, 1);
            ctx->render_square(matrix4::translate(monster->action.x - monster->x,
                                                  monster->action.y - monster->y,
                                                  -gs->selected_entity_height) * m,
                color, SHADER_COLOR);
        }
    }
}

void render_battle_queue(context *ctx, game_state *gs, input_state *)
{
    int x = 200;
    int y = 20;
    // uint32 queue_index = gs->turn_no;

    for (int i = 0; i < gs->battle_queue.size(); i++)
    {
        auto eid = gs->battle_queue[i];

        float32 scale = 10.f;
        auto color = V4(0.4, 0.4, 0.4, 1);
        if (i == gs->battle_queue_current_slot)
        {
            scale = 11.f;
            color = V4(0.6, 0.6, 0.8, 1);
        }

        ctx->render_ui(matrix4::translate(x, y, 0) * matrix4::scale(scale, scale, 1), color);

        auto string_buffer = ctx->temporary_allocator.allocate_buffer(32);
        auto str = make_array<char>(string_buffer);
        int n = eid.id;
        while (n > 0)
        {
            str.insert(str.begin(), (n % 10) + '0');
            n /= 10;
        }

        ctx->render_text(
                matrix4::translate(x - 9, y + 25, -0.1) * matrix4::scale(0.8, 0.8, 1),
                V4(1), str.data());

        x += 25;
    }
}

void render_timer(context *ctx, game_state *gs, input_state *input)
{
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
}


} // namespace game

#endif // GAME_SYSTEMS_RENDER_HPP

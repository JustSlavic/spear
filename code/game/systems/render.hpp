#ifndef GAME_SYSTEMS_RENDER_HPP
#define GAME_SYSTEMS_RENDER_HPP


namespace game {


void setup_render_camera(context *ctx, game_state *gs, input_state *)
{
    ctx->setup_camera(gs->camera.position, gs->camera.forward, gs->camera.up);
}

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

void draw_map_2(context *ctx, game_state *gs, input_state *)
{
    {
        auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
        snprintf((char *) buffer.data, 63,
            "intersect ijk = %d, %d, %d", gs->intersect_tile.x, gs->intersect_tile.y, gs->intersect_tile.z);
        ctx->render_text(matrix4::translate(10.f, 150.f, 0.f), V4(1), (char const *) buffer.data);
    }
    {
        auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
        snprintf((char *) buffer.data, 63,
            "intersect t = %f", gs->intersect_t);
        ctx->render_text(matrix4::translate(10.f, 200.f, 0.f), V4(1), (char const *) buffer.data);
    }
    {
        auto c = V4(1, 0, 1, 1);
        auto m = matrix4::translate(gs->intersection)
               * matrix4::scale(0.05f);
        ctx->render_cube(m, c, RenderShader_SingleColor);
    }

    for (int k = 0; k < gs->map.dim.z; k++)
    {
        for (uint32 j = 0; j < gs->map.dim.y; j++)
        {
            for (uint32 i = 0; i < gs->map.dim.x; i++)
            {
                if (gs->map.get(i, j, k) == GameMapOccupation_Ground)
                {
                    auto c = V4((float32) i / gs->map.dim.x,
                                (float32) j / gs->map.dim.y,
                                (float32) k / gs->map.dim.z,
                                1);
                    if (gs->intersected && gs->intersect_tile == make_vector3i(i, j, k))
                    {
                        c = V4(c.rgb * 1.2f, 1.0f);
                    }

                    float32 x = (float32) i - (float32) gs->map.origin.x;
                    float32 y = (float32) j - (float32) gs->map.origin.y;
                    float32 z = (float32) k - (float32) gs->map.origin.z;
                    auto m = matrix4::translate(x, y, z) * matrix4::scale(0.45f);
                    ctx->render_cube(m, c, RenderShader_Ground);
                }
            }
        }
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

        float32 startP = (float32) (-(hpBarWidth / 2 + gap / 2) * (e->max_hp - 1));
        for (int i = 0; i < e->max_hp; i++)
        {
            auto color = i < e->hp ? V4(1, 0.2, 0.1, 1) : V4(0.3, 0.3, 0.3, 1);
            ctx->render_banner(V3(x, y, z + 1),
                matrix4::translate(startP + i * (hpBarWidth + gap), 0, 0) *
                matrix4::scale(hpBarWidth / 2.f, hpBarWidth / 2.f, 1.f)
                , color);
        }
    }
}

void render_hero(context *ctx, game_state *gs, input_state *)
{
    entity *hero = game::get_entity(gs, gs->hero_eid);
    if (hero)
    {
        float32 x = (float32) hero->tile.x - gs->map.origin.x;
        float32 y = (float32) hero->tile.y - gs->map.origin.y;
        float32 z = (float32) hero->tile.z - gs->map.origin.z;
        // float height = hero->eid == gs->selected_entity_eid ? gs->selected_entity_height
        //              : gs->regular_entity_height;
        auto m = matrix4::translate(x, y, z) *
                 matrix4::scale(0.35f, 0.35f, 0.35f);

        ctx->render_cube(m, V4(1, 1, 1, 1), RenderShader_SingleColor);
        draw_health_bar(ctx, hero, x, y, z);
    }
}

void render_monsters(context *ctx, game_state *gs, input_state *)
{
    for (int i = 0; i < gs->monsters.size(); i++)
    {
        entity *monster = game::get_entity(gs, gs->monsters[i]);
        vector3 p = to_vector3(monster->tile - gs->map.origin);
        // float height = monster->eid == gs->selected_entity_eid ? gs->selected_entity_height
        //              : gs->regular_entity_height;
        auto m = matrix4::translate(p) *
                 matrix4::scale(0.35f, 0.35f, 0.35f);

        ctx->render_cube(m, V4(0.9, 0.4, 0.2, 1), RenderShader_SingleColor);
        draw_health_bar(ctx, monster, p.x, p.y, p.z);

        // if (monster->action.kind != ENTITY_ACTION_NONE)
        // {
        //     auto color = monster->action.kind == ENTITY_ACTION_MOVE ? gs->move_color :
        //                  monster->action.kind == ENTITY_ACTION_LEFT_ARM ? gs->defence_color :
        //                  monster->action.kind == ENTITY_ACTION_RIGHT_ARM ? gs->attack_color :
        //                  V4(1, 1, 1, 1);
        //     ctx->render_square(matrix4::translate((float32) (monster->action.x - monster->x),
        //                                           (float32) (monster->action.y - monster->y),
        //                                           (float32) (-gs->selected_entity_height)) * m,
        //         color, RenderShader_SingleColor);
        // }
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

        // float32 scale = 10.f;
        // auto color = V4(0.4, 0.4, 0.4, 1);
        // ctx->render_ui(matrix4::translate((float32) x, (float32) y, 0.f) * matrix4::scale(10, 10, 1), color);

        auto string_buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 32);
        auto str = make_array<char>(string_buffer);
        int n = eid.id;
        while (n > 0)
        {
            str.insert(str.begin(), (n % 10) + '0');
            n /= 10;
        }

        ctx->render_text(
            matrix4::translate(x - 9.f, y + 50.f, 0.f) * matrix4::scale(2),
            V4(1), str.data());
        x += 40;
    }
}

void render_timer(context *ctx, game_state *gs, input_state *input)
{
    if (gs->turn_timer_enabled)
    {
        float32 t = (float32) (1.f - (input->time - gs->turn_start_time) / gs->seconds_for_turn);
        vector4 color = V4(sin((t - 3) * pi * 0.5f),
                           -cos((t + 1) * pi * 0.5f),
                           0, 1);
        ctx->render_square(
                       matrix4::translate_y((float32) (ctx->viewport.height - 10)) *
                       matrix4::scale(ctx->viewport.width * t, 2, 1)
            , color, RenderShader_SingleColor);
    }
}


void render_dialogue(context *ctx, game_state *gs, input_state *input)
{
    ctx->render_text(
        matrix4::translate(100.f, 240.f, 0.f),
        V4(1), "Hello, world!");
}

void render_camera_position(context *ctx, game_state *gs)
{
    auto buffer = ALLOCATE_BUFFER(ctx->temporary_allocator, 64);
    auto p = gs->camera.position;
    snprintf((char *) buffer.data, 63, "Camera.P = %4.2f, %4.2f, %4.2f", p.x, p.y, p.z);
    ctx->render_text(matrix4::translate(10.f, 100.f, 0.f), V4(1), (char const *) buffer.data);
}




} // namespace game

#endif // GAME_SYSTEMS_RENDER_HPP

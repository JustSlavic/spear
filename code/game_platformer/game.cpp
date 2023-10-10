#include "game.hpp"
#include <game_interface.hpp>


// @todo:
// + packages
// + pick up packages (backpack)
// + on hit stone loose packages
// + on hit stone teleport onto this stone with the signature blinking
// + sent packages via postbox
// + draw the score number as roman numerals


#include <math/integer.hpp>
#include <math/float64.hpp>
#include <math/rectangle2.hpp>
#include <g2.hpp>
#include <g301.hpp>

#include <collision.hpp>
#include <image/png.hpp>

#include <g2.hpp>

#ifndef osOutputDebugString
#if OS_WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define osOutputDebugString(MSG, ...) \
{  \
    char OutputBuffer_##__LINE__[256]; \
    sprintf(OutputBuffer_##__LINE__, MSG, __VA_ARGS__); \
    OutputDebugStringA(OutputBuffer_##__LINE__); \
} void(0)
#elif OS_LINUX
#define osOutputDebugString(MSG, ...) printf(MSG, __VA_ARGS__)
#elif OS_MAC
#define osOutputDebugString(MSG, ...)
#endif // OS_*
#endif


float uniform_real(float from, float to)
{
    ASSERT(to > from);

    float r = float(rand()) / float(RAND_MAX / to); // Uniform [0, to]
    return r + from;
}
int32 uniform_int(int32 from, int32 to)
{
    ASSERT(to > from);

    int r = rand() % (to - from) + to;
    return r;
}

#if DEBUG
debug_time_measurement *global_debug_measurements;
uint32 global_debug_call_depth;
#endif

GLOBAL vector4 sky_color = V4(148.0 / 255.0, 204.0 / 255.0, 209.0 / 255.0, 1.0);
GLOBAL vector4 porter_color = V4(55.0/255.0, 70.0/255.0, 122.0/255.0, 1);
GLOBAL vector4 ground_color = V4(50.0/255.0, 115.0/255.0, 53.0/255.0, 1);
GLOBAL vector4 stones_color = V4(184.0/255.0, 165.0/255.0, 136.0/255.0, 1);
GLOBAL vector4 package_color = V4(255.0/255.0, 255.0/255.0, 0.0/255.0, 1);
GLOBAL vector4 lou_color = V4(247.0/255.0, 180.0/255.0, 54.0/255.0, 1);
GLOBAL vector2 gravity = V2(0, -9.8); // m/s^2

#define NEAR_EXIT_TIME_SECONDS            1.0f
#define PLAYER_BASE_MOVEMENT_ACCELERATION 50.0f
#define PLAYER_BASE_JUMP_ACCELERATION     5.0f
#define PLAYER_BASE_JUMP_CORRECTION_SPEED 0.2f
#define BASE_FRICTION_COEFFICIENT         5.f


INLINE entity_ref push_entity(game_state *gs)
{
    ASSERT(gs->entity_count < gs->entities_capacity);
    entity_ref result;
    result.eid = (uint32) gs->entity_count++;
    result.e = gs->entities + result.eid;
    return result;
}

INLINE entity *get_entity(game_state *gs, uint32 eid)
{
    ASSERT(eid < gs->entity_count);

    entity *result = gs->entities + eid;
    return result;
}


ui::handle make_push_button(game_state *gs, vector2 position)
{
    auto button = ui::make_group(gs->hud);
    ui::set_position(gs->hud, button, position);
    ui::make_hoverable(gs->hud, button);
    auto click_callbacks_4 = ui::make_clickable(gs->hud, button);
    click_callbacks_4->on_press_internal = [](ui::system *s, ui::handle h)
    {
        auto it = ui::iterate_attaches(s, h);

        ui::set_visible(s, it[2], false);
        ui::set_visible(s, it[3], true);
    };
    click_callbacks_4->on_release_internal = [](ui::system *s, ui::handle h)
    {
        auto it = ui::iterate_attaches(s, h);

        ui::set_visible(s, it[2], true);
        ui::set_visible(s, it[3], false);
    };

    auto button_3_state_up = ui::make_image(gs->hud, button);
    ui::set_visible(gs->hud, button_3_state_up, true);
    ui::set_texture(gs->hud, button_3_state_up, gs->button_push_1_texture);
    auto button_3_state_down = ui::make_image(gs->hud, button);
    ui::set_visible(gs->hud, button_3_state_down, false);
    ui::set_texture(gs->hud, button_3_state_down, gs->button_push_2_texture);

    return button;
}

rs::resource_token load_texture_resource_from_file(execution_context *context, char const *filename)
{
    rs::resource_token result = {};

    memory_block file_content = context->debug_load_file(context->temporary_allocator, filename);
    if (file_content.memory != NULL)
    {
        auto bitmap = image::load_png(context->temporary_allocator, context->temporary_allocator, file_content);
        if (bitmap.pixels != NULL)
        {
            result = rs::create_texture_resource(&context->resource_storage, bitmap);
        }
    }

    return result;
}


INITIALIZE_MEMORY_FUNCTION(execution_context *context, memory_block game_memory)
{
    using namespace math;

    srand((unsigned int)time(NULL));

    global_debug_measurements = context->debug_measurements;

    ASSERT(sizeof(game_state) < game_memory.size);
    ASSERT(context->debug_load_file);

    memory_allocator game_allocator = memory_allocator__create_arena_from_memory_block(game_memory);
    game_state *gs = ALLOCATE(game_allocator, game_state);
    gs->game_allocator = game_allocator;

    context->game_state = gs;

    // Entities
    {
        gs->entities_capacity = 20;
        memory_block entities = ALLOCATE_BUFFER_ALIGNED(gs->game_allocator, sizeof(entity) * gs->entities_capacity, alignof(entity));
        gs->entities = (entity *) entities.memory;
        // @note: let zero-indexed entity be 'null entity' representing lack of entity
        gs->entity_count = 1;
    }

    gs->default_camera.position = V3(0, 0, 20);

    // Init rectangle mesh
    {
        float32 vbo_init[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
        };

        uint32 ibo_init[] = {
            0, 1, 2, // first triangle
            2, 3, 0, // second triangle
        };

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        auto vbo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(vbo_init));
        memory__copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(ibo_init));
        memory__copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->rectangle_mesh = create_mesh_resource(&context->resource_storage, vbo, ibo, vbl);
        gs->rectangle_shader = create_shader_resource(&context->resource_storage, STRID("rectangle.shader"));
    }

    // Rectangle with UV
    {
        float32 vbo_init[] = {
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
        };

        uint32 ibo_init[] =
        {
            0, 1, 2, // first triangle
            2, 3, 0, // second triangle
        };

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3); // XYZ
        gfx::push_layout_element(&vbl, 2); // UV

        auto vbo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(vbo_init));
        memory__copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(ibo_init));
        memory__copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->rectangle_mesh_uv   = create_mesh_resource(&context->resource_storage, vbo, ibo, vbl);
        gs->rectangle_shader_uv = create_shader_resource(&context->resource_storage, STRID("rectangle_uv.shader"));
    }

    // Load textures
    gs->button_push_1_texture = load_texture_resource_from_file(context, "button_push_1.png");
    gs->button_push_2_texture = load_texture_resource_from_file(context, "button_push_2.png");

    // UI
    {
        auto ui_memory = ALLOCATE_BUFFER_(gs->game_allocator, MEGABYTES(1));
        gs->hud = ui::initialize(ui_memory);
#if UI_EDITOR_ENABLED
        gs->ui_editor = ui::initialize_editor(gs->game_allocator);
#endif // UI_EDITOR_ENABLED

        ui::set_string_id_storage(gs->hud, context->strid_storage);
        ui::set_resource_rectangle_mesh(gs->hud, gs->rectangle_mesh);
        ui::set_resource_rectangle_shader(gs->hud, gs->rectangle_shader);
        ui::set_resource_rectangle_mesh_uv(gs->hud, gs->rectangle_mesh_uv);
        ui::set_resource_rectangle_shader_uv(gs->hud, gs->rectangle_shader_uv);

        auto button_3 = make_push_button(gs, V2(400, 400));
        ui::set_scale(gs->hud, button_3, V2(3, 3));

        auto button_4 = make_push_button(gs, V2(700, 600));

        auto button_1 = ui::make_group(gs->hud);
        auto shape_1 = ui::make_shape(gs->hud, button_1);
        auto hover_callbacks_1 = ui::make_hoverable(gs->hud, button_1);
        auto click_callbacks_1 = ui::make_clickable(gs->hud, button_1);

        hover_callbacks_1->on_enter_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "shape_1.on_hover.color.r");
            ui::play_animation(s, "shape_1.on_hover.color.g");
        };

        hover_callbacks_1->on_leave_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "shape_1.on_leave.color.r");
            ui::play_animation(s, "shape_1.on_leave.color.g");
        };

        click_callbacks_1->on_press_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "button_1.on_click.rotation");
        };

        click_callbacks_1->on_release_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "button_1.on_release.rotation");
        };

        ui::set_position(gs->hud, button_1, V2(100, 100));
        ui::set_color(gs->hud, shape_1, V4(0, 0, 0, 1));
        ui::set_rotation(gs->hud, shape_1, 45.f);
    }
}


struct collision_result
{
    vector2 normal; // The normal of the surface entity collided with
    vector2 point; // The position of entity when the collision hapened
    float32 t; // The distance in meters entity traversed until collision
};


// Assume that collision volumes of both entities are AABBs
bool32 do_collision(entity *e1, entity *e2, vector2 p1, vector2 p2, collision_result *result)
{
    ASSERT(is_not(e1, ENTITY_INVALID));
    ASSERT(is_not(e2, ENTITY_INVALID));

    bool32 collided = false;

    auto minkowski_radius_x = .5f * (e1->width  + e2->width);
    auto minkowski_radius_y = .5f * (e1->height + e2->height);

    auto tl = V2(e2->position.x - minkowski_radius_x, e2->position.y + minkowski_radius_y);
    auto bl = V2(e2->position.x - minkowski_radius_x, e2->position.y - minkowski_radius_y);
    auto tr = V2(e2->position.x + minkowski_radius_x, e2->position.y + minkowski_radius_y);
    auto br = V2(e2->position.x + minkowski_radius_x, e2->position.y - minkowski_radius_y);

    vector2 vertices[5] = { bl, tl, tr, br, bl };

    float32 min_distance = math::infinity;

    for (uint32 i = 0; i < ARRAY_COUNT(vertices) - 1; i++)
    {
        auto w1 = vertices[i];
        auto w2 = vertices[i + 1];
        auto wall = (w2 - w1);
        auto normal = normalized(V2(-wall.y, wall.x));

        if (inner(p2 - p1, normal) < 0.f)
        {
            float32 t1, t2;
            vector2 c1, c2;
            float sq_distance = sq_distance_segment_segment(p1, p2, w1, w2, t1, t2, c1, c2);

            if ((sq_distance < EPSILON) && (t1 < min_distance))
            {
                result->normal = normal;
                result->point = c1;
                result->t = t1;
                min_distance = t1;
                collided = true;
            }
        }
    }

    return collided;
}


UPDATE_AND_RENDER_FUNCTION(execution_context *context, memory_block game_memory, input_state input)
{
    using namespace math;

    float32 dt = input->dt;
    global_debug_measurements = context->debug_measurements;

    game_state *gs = (game_state *) context->game_state;
    sam_move move_data = {};

    if (get_press_count(input->keyboard[KB_ESC]))
    {
        if (gs->near_exit_time > 0.f)
        {
            push_execution_command(context, exit_command());
        }
        else
        {
            gs->near_exit_time = NEAR_EXIT_TIME_SECONDS;
        }
    }

#if UI_EDITOR_ENABLED
    if (get_press_count(input->keyboard[KB_F1]))
    {
        TOGGLE(gs->ui_editor_enabled);
    }
#endif // UI_EDITOR_ENABLED

    // Setup camera
    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->default_camera.position;
        setup_camera.look_at_position = V3(gs->default_camera.position.x, gs->default_camera.position.y, 0);
        setup_camera.camera_up_direction = V3(0, 1, 0);

        push_setup_camera_command(context, setup_camera);
    }

    // Background
    {
        render_command::command_draw_background draw_background;
        draw_background.mesh = gs->rectangle_mesh;
        draw_background.shader = gs->rectangle_shader;
        draw_background.color = sky_color;
        push_draw_background_command(context, draw_background);
    }

    // Coordinates
    // {
    //     // X axis
    //     draw_aligned_rectangle(context, gs, 0.5f, 0.f, 0.5f, 0.05f, V4(0.9, 0.2, 0.2, 1.0));
    //     // Y axis
    //     draw_aligned_rectangle(context, gs, 0.f, 0.5f, 0.05f, 0.5f, V4(0.2, 0.9, 0.2, 1.0));
    // }

    for (uint32 eid = 1; eid < gs->entity_count; eid++)
    {
        entity *e = get_entity(gs, eid);

        e->collided = false;

        if (is_not(e, ENTITY_STATIC))
        {
            float32 dt_ = dt;
            for (int move = 0; move < 1; move++)
            {
                auto old_v = e->velocity;
                auto old_p = e->position;

                auto acceleration = gravity;
                if (is(e, ENTITY_SAM))
                {
                    acceleration += move_data.acceleration;
                    if (is(e, ENTITY_ON_GROUND) && move_data.jump)
                    {
                        unset(e, ENTITY_ON_GROUND);
                    }
                    else
                    {
                        e->velocity += move_data.velocity;
                    }
                }

                // @todo: should I consider air resistance?
                if (is(e, ENTITY_ON_GROUND))
                {
                    // Friction with the ground
                    // auto normal_force = e->mass * math::length(gravity);
                    auto friction = (BASE_FRICTION_COEFFICIENT * (-old_v));
                    acceleration += friction;
                }

                auto new_v = e->velocity + acceleration * dt_;
                auto new_p = e->position + 0.5f * (old_v + new_v) * dt_;

                auto full_distance = length(new_p - old_p);
                auto distance = full_distance;
                if (distance < EPSILON) break;

                unset(e, ENTITY_ON_GROUND);
                if (is(e, ENTITY_COLLIDABLE) || is(e, ENTITY_INTERACTABLE))
                {
                    for (uint32 test_eid = 1; test_eid < gs->entity_count; test_eid++)
                    {
                        // Do not self collide!
                        if (eid == test_eid) continue;

                        entity *e2 = get_entity(gs, test_eid);
                        if (is_not(e2, ENTITY_COLLIDABLE) && is_not(e2, ENTITY_INTERACTABLE)) continue;

                        collision_result collision;
                        bool32 collided = do_collision(e, e2, old_p, new_p, &collision);
                        if (collided)
                        {
                            if (is(e2, ENTITY_COLLIDABLE))
                            {
                                // Collision of the player with the ground
                                e->collided = true;
                                set(e, ENTITY_ON_GROUND);
                                new_p = collision.point + collision.normal * 0.01f;
                                new_v = new_v - inner(new_v, collision.normal) * collision.normal;
                            }
                        }
                    }
                }

                e->position = new_p;
                e->velocity = new_v;

                dt_ = (dt_ - dt_ * (distance / full_distance));
                if (dt_ < EPSILON) break;
            }
        }
    }

#if UI_EDITOR_ENABLED
    if (gs->ui_editor_enabled)
    {
        ui::update_editor(gs->hud, gs->ui_editor, input);
        // Reset s-> hot, active, pressed
    }
    else
    {
        ui::update(gs->hud, input);
    }
    ui::render(context, gs->hud);
    if (gs->ui_editor_enabled)
    {
        ui::render_editor(context, gs->hud, gs->ui_editor);

        {
            render_command::command_draw_screen_frame draw_frame;
            draw_frame.model = matrix4__identity();
            draw_frame.view = matrix4__identity();
            draw_frame.projection = matrix4__identity();
            draw_frame.color = V4(0,0,0,1);
            push_draw_screen_frame(context, draw_frame);
        }
    }
#else // UI_EDITOR_ENABLED
    ui::update(gs->hud, input);
    ui::render(context, gs->hud);
#endif // UI_EDITOR_ENABLED

    if (gs->near_exit_time > 0)
    {
        render_command::command_draw_screen_frame draw_frame;
        draw_frame.model = matrix4__identity();
        draw_frame.view = matrix4__identity();
        draw_frame.projection = matrix4__identity();
        draw_frame.color = V4(1,0,0,1);
        push_draw_screen_frame(context, draw_frame);

        gs->near_exit_time -= dt;
        if (gs->near_exit_time < 0)
            gs->near_exit_time = 0;
    }
}

#if DLL_BUILD
#include <memory_allocator.c>
#include <string_id.cpp>
#include <rs/resource_system.cpp>
#include <image/bmp.cpp>
#include <image/png.cpp>
#include <crc.cpp>
#endif // DLL_BUILD

#include <execution_context.cpp>
#include <ui/ui.cpp>

#if UI_EDITOR_ENABLED
#include <ui/ui_editor.cpp>
#endif // UI_EDITOR_ENABLED

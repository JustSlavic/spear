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
#include <math/vector4.hpp>
#include <math/matrix2.hpp>
#include <math/matrix4.hpp>

#include <collision.hpp>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <image/png.hpp>

#if OS_WINDOWS
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

GLOBAL float32 package_width = 0.9f;
GLOBAL float32 package_height = 0.2f;
GLOBAL float32 lou_width = 0.3f;
GLOBAL float32 lou_height = 0.5f;


GLOBAL float32 letter_width = 0.5f;
GLOBAL float32 letter_height = 0.8f;
GLOBAL float32 spacing = 0.4f;


GLOBAL math::vector4 sky_color = V4(148.0 / 255.0, 204.0 / 255.0, 209.0 / 255.0, 1.0);
GLOBAL math::vector4 porter_color = V4(55.0/255.0, 70.0/255.0, 122.0/255.0, 1);
GLOBAL math::vector4 ground_color = V4(50.0/255.0, 115.0/255.0, 53.0/255.0, 1);
GLOBAL math::vector4 stones_color = V4(184.0/255.0, 165.0/255.0, 136.0/255.0, 1);
GLOBAL math::vector4 package_color = V4(255.0/255.0, 255.0/255.0, 0.0/255.0, 1);
GLOBAL math::vector4 lou_color = V4(247.0/255.0, 180.0/255.0, 54.0/255.0, 1);
GLOBAL math::vector2 gravity = V2(0, -9.8); // m/s^2

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

void draw_aligned_rectangle(execution_context *context, game_state *gs, float32 x, float32 y, float32 half_width, float32 half_height, math::vector4 color)
{
    render_command::command_draw_mesh_with_color draw_aligned_rectangle;
    draw_aligned_rectangle.mesh_token = gs->rectangle_mesh;
    draw_aligned_rectangle.shader_token = gs->rectangle_shader;
    draw_aligned_rectangle.model =
        math::translated(V3(x, y, 0),
        math::scaled(V3(half_width, half_height, 1),
            math::matrix4::identity()));
    draw_aligned_rectangle.color = color;

    push_draw_mesh_with_color_command(context, draw_aligned_rectangle);
}

void teleport_back(game_state *gs)
{
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *e = get_entity(gs, entity_index);
        e->position.x -= 100.f;
    }
    gs->default_camera.position.x -= 100.f;
}

void sprinkle_stones(game_state *gs)
{
    float32 x = gs->sam->position.x;
    uint32 sprinkled = 0;
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *e = get_entity(gs, entity_index);
        if ((e->type == ENTITY_STONE) && (e->position.x < x - 20.f))
        {
            float32 x_mean = (sprinkled + 3) * 3.f;
            e->position = V2(uniform_real(x_mean - 1.f, x_mean + 1.f) + 20.0f, -4. + e->height * 0.5f - uniform_real(0.1f, 0.3f));
            sprinkled += 1;
        }
    }
}


void sprinkle_packages(game_state *gs)
{
    auto x = gs->sam->position.x;
    uint32 sprinkled = 0;
    for (uint32 entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *e = get_entity(gs, entity_index);
        if ((e->type == ENTITY_PACKAGE) && (e->position.x < x - 20.f))
        {
            float32 x_mean = (sprinkled + 3) * 3.f;
            e->position = V2(uniform_real(x_mean - 1.f, x_mean + 1.f) + 20.0f, -2);
            e->deleted = false;
            sprinkled += 1;
        }
    }
}


//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    using namespace math;

    srand((unsigned int)time(NULL));

    global_debug_measurements = context->debug_measurements;

    ASSERT(sizeof(game_state) < game_memory.size);
    game_state *gs = (game_state *) game_memory.memory;

    memory::initialize_memory_arena(&gs->game_allocator, (byte *) game_memory.memory + sizeof(game_state), game_memory.size - sizeof(game_state));

    // Entities
    {
        gs->entities = (entity *) ALLOCATE_BUFFER_(&gs->game_allocator, sizeof(entity) * 20);
        gs->entities_capacity = 200000;
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

        auto vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(vbo_init));
        memory::copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(ibo_init));
        memory::copy(ibo.memory, ibo_init, sizeof(ibo_init));

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

        auto vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(vbo_init));
        memory::copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(ibo_init));
        memory::copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->rectangle_mesh_uv   = create_mesh_resource(&context->resource_storage, vbo, ibo, vbl);
        gs->rectangle_shader_uv = create_shader_resource(&context->resource_storage, STRID("rectangle_uv.shader"));
    }

    // Load texture
    {
        memory_block file_content = context->debug_load_file(&context->temporary_allocator, "IMG_1308.bmp");
        gs->reference_texture = rs::create_texture_resource(&context->resource_storage, file_content);
    }

    {
        memory_block file_content = context->debug_load_file(&context->temporary_allocator, "pepe.png");
        auto bitmap = image::load_png(&context->temporary_allocator, &context->temporary_allocator, file_content);
    }

    // UI
    {
        auto ui_memory = ALLOCATE_BLOCK_(&gs->game_allocator, MEGABYTES(1));
        gs->hud = ui::initialize(ui_memory);

        ui::set_string_id_storage(gs->hud, context->strid_storage);
        ui::set_resource_rectangle_mesh(gs->hud, gs->rectangle_mesh);
        ui::set_resource_rectangle_shader(gs->hud, gs->rectangle_shader);

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
        ui::set_color(gs->hud, shape_1, V4(1, 0, 0, 1));

        ui::animate(gs->hud, button_1, STRID("button_1.on_click.rotation"), UI_ANIM_ROTATION, 5, 0.f, 30.f);
        ui::animate(gs->hud, button_1, STRID("button_1.on_release.rotation"), UI_ANIM_ROTATION, 5, 30.f, 0.f);

        ui::animate(gs->hud, shape_1, STRID("shape_1.on_hover.color.r"), UI_ANIM_COLOR_R, 20, 1.f, 0.5f);
        ui::animate(gs->hud, shape_1, STRID("shape_1.on_hover.color.g"), UI_ANIM_COLOR_G, 10, 0.f, 0.8f);
        ui::animate(gs->hud, shape_1, STRID("shape_1.on_leave.color.r"), UI_ANIM_COLOR_R, 20, 0.5f, 1.f);
        ui::animate(gs->hud, shape_1, STRID("shape_1.on_leave.color.g"), UI_ANIM_COLOR_G, 10, 0.8f, 0.f);

        auto button_2 = ui::make_group(gs->hud);
        ui::set_position(gs->hud, button_2, V2(500, 600));
        ui::set_rotation(gs->hud, button_2, 20.f);

        auto shape_2 = ui::make_shape(gs->hud, button_2);
        ui::set_color(gs->hud, shape_2, V4(0.9, 0.4, 0.2, 1.0));
        auto hover_callbacks_2 = ui::make_hoverable(gs->hud, button_2);
        auto click_callbacks_2 = ui::make_clickable(gs->hud, button_2);

        ui::animate(gs->hud, shape_2, STRID("shape_2.on_enter.color.g"), UI_ANIM_COLOR_G | UI_ANIM_PPONG, 40, 0.4f, 0.6f);
        hover_callbacks_2->on_enter_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "shape_2.on_enter.color.g");
        };
        ui::animate(gs->hud, shape_2, STRID("shape_2.on_leave.color.g"), UI_ANIM_COLOR_G, 40, 0.6f, 0.4f);
        hover_callbacks_2->on_leave_internal = [](ui::system *s, ui::handle h)
        {
            ui::stop_animation(s, "shape_2.on_enter.color.g");
            ui::play_animation(s, "shape_2.on_leave.color.g");
        };

        ui::animate(gs->hud, shape_2, STRID("shape_2.on_press.position.x"), UI_ANIM_POSITION_X, 3, 0, 5);
        ui::animate(gs->hud, shape_2, STRID("shape_2.on_press.position.y"), UI_ANIM_POSITION_Y, 3, 0, 5);
        click_callbacks_2->on_press_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "shape_2.on_press.position.x");
            ui::play_animation(s, "shape_2.on_press.position.y");
        };
        ui::animate(gs->hud, shape_2, STRID("shape_2.on_release.position.x"), UI_ANIM_POSITION_X, 3, 5, 0);
        ui::animate(gs->hud, shape_2, STRID("shape_2.on_release.position.y"), UI_ANIM_POSITION_Y, 3, 5, 0);
        click_callbacks_2->on_release_internal = [](ui::system *s, ui::handle h)
        {
            ui::play_animation(s, "shape_2.on_release.position.x");
            ui::play_animation(s, "shape_2.on_release.position.y");
        };

        auto shape_3 = ui::make_shape(gs->hud);
        ui::set_position(gs->hud, shape_3, V2(400, 200));
        ui::set_scale(gs->hud, shape_3, V2(5, 5));
        ui::set_rotation(gs->hud, shape_3, 70.f);
        ui::set_color(gs->hud, shape_3, V4(0.3, 0.3, 0.8, 1.0));

        ui::animate(gs->hud, shape_3, STRID("shape_3.ppong.position.x"), UI_ANIM_POSITION_X | UI_ANIM_PPONG, 120, 0, 100);
        ui::play_animation(gs->hud, "shape_3.ppong.position.x");

        auto hover_callbacks_3 = ui::make_hoverable(gs->hud, shape_3);
        hover_callbacks_3->on_enter_internal = [](ui::system *s, ui::handle h)
        {
            ui::set_color(s, h, V4(1, 0, 0, 0));
        };
        hover_callbacks_3->on_leave_internal = [](ui::system *s, ui::handle h)
        {
            ui::set_color(s, h, V4(0.3, 0.3, 0.8, 1.0));
        };
    }
}


struct collision_result
{
    math::vector2 normal; // The normal of the surface entity collided with
    math::vector2 point; // The position of entity when the collision hapened
    float32 t; // The distance in meters entity traversed until collision
};


// Assume that collision volumes of both entities are AABBs
bool32 do_collision(entity *e1, entity *e2, math::vector2 p1, math::vector2 p2, collision_result *result)
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

    math::vector2 vertices[5] = { bl, tl, tr, br, bl };

    float32 min_distance = math::infinity;

    for (uint32 i = 0; i < ARRAY_COUNT(vertices) - 1; i++)
    {
        auto w1 = vertices[i];
        auto w2 = vertices[i + 1];
        auto wall = (w2 - w1);
        auto normal = normalized(V2(-wall.y, wall.x));

        if (dot(p2 - p1, normal) < 0.f)
        {
            float32 t1, t2;
            math::vector2 c1, c2;
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


//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
// - input_devices input;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{
    using namespace math;

    float32 dt = input->dt;
    global_debug_measurements = context->debug_measurements;

    DEBUG_BEGIN_TIME_MEASUREMENT(update_and_render);

    game_state *gs = (game_state *) game_memory.memory;
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

    if (get_press_count(input->keyboard[KB_F1]))
    {
        TOGGLE(gs->ui_editor_enabled);
    }

    if (get_hold_count(input->keyboard[KB_A]))
    {
        if (is(gs->sam, ENTITY_ON_GROUND))
        {
            move_data.acceleration.x -= PLAYER_BASE_MOVEMENT_ACCELERATION;
        }
        else
        {
            move_data.velocity.x -= PLAYER_BASE_JUMP_CORRECTION_SPEED;
        }
    }
    if (get_hold_count(input->keyboard[KB_D]))
    {
        if (is(gs->sam, ENTITY_ON_GROUND))
        {
            move_data.acceleration.x += PLAYER_BASE_MOVEMENT_ACCELERATION;
        }
        else
        {
            move_data.velocity.x += PLAYER_BASE_JUMP_CORRECTION_SPEED;
        }
    }

    if (get_press_count(input->keyboard[KB_SPACE]))
    {
        if (is(gs->sam, ENTITY_ON_GROUND))
        {
            move_data.acceleration.y += (PLAYER_BASE_JUMP_ACCELERATION / (0.1f * gs->carried_packages + 1)) / dt;
            move_data.jump = true;
        }
    }

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
    {
        // X axis
        draw_aligned_rectangle(context, gs, 0.5f, 0.f, 0.5f, 0.05f, V4(0.9, 0.2, 0.2, 1.0));
        // Y axis
        draw_aligned_rectangle(context, gs, 0.f, 0.5f, 0.05f, 0.5f, V4(0.2, 0.9, 0.2, 1.0));
    }

    {
        render_command::command_draw_mesh_with_texture cmd;
        cmd.mesh_token = gs->rectangle_mesh_uv;
        cmd.shader_token = gs->rectangle_shader_uv;
        cmd.texture_token = gs->reference_texture;

        cmd.model = math::matrix4::identity();
        push_draw_mesh_with_texture_command(context, cmd);
    }

    // if (ui::button(&gs->ui, STRID("Button1").id))
    // {
    //     osOutputDebugString("PRESSED!!!\n");
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

                // @todo: should I consider air resitance?
                if (is(e, ENTITY_ON_GROUND))
                {
                    // Friction with the ground
                    auto normal_force = e->mass * math::length(gravity);
                    auto friction = (BASE_FRICTION_COEFFICIENT * (-old_v));
                    acceleration += friction;
                }

                auto new_v = e->velocity + acceleration * dt_;
                auto new_p = e->position + 0.5f * (old_v + new_v) * dt_;

                auto full_distance = math::length(new_p - old_p);
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
                                new_v = new_v - dot(new_v, collision.normal) * collision.normal;
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

        continue;
        switch (e->type)
        {
            case ENTITY_SAM:
            {
                float32 blink_phase = gs->blink_time / gs->blink_freq;
                if (((int) blink_phase) % 2 > 0) continue;

                draw_aligned_rectangle(
                    context, gs,
                    e->position.x, e->position.y,
                    e->width * 0.5f, e->height * 0.5f,
                    e->collided ? V4(1, 0, 0, 1) :
                    is(e, ENTITY_ON_GROUND) ? V4(0, 0.8, 0.5, 1) :
                    porter_color);

                draw_aligned_rectangle(
                    context, gs,
                    e->position.x + 0.05f, e->position.y + 1.0f,
                    e->width * 0.5f, 0.35f,
                    porter_color);

                draw_aligned_rectangle(
                    context, gs,
                    e->position.x + 0.2f, e->position.y + 1.0f,
                    0.05f, 0.3f,
                    V4(242.0/255.0, 242.0/255.0, 218.0/255.0, 1));
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x - 0.1f, e->position.y + 0.2f,
                    0.025f, 0.4f,
                    V4(0.9, 0.2, 0.2, 1.0));
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x, e->position.y - 0.4f,
                    0.025f, 0.5f,
                    V4(0.9, 0.2, 0.2, 1.0));

                draw_aligned_rectangle(
                    context, gs,
                    e->position.x + 0.1f, e->position.y - 0.85f,
                    0.2f, 0.1f,
                    V4(0, 0, 0, 1));

                // Lou
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x + 0.3f, e->position.y + 0.06f,
                    lou_width * 0.5f, lou_height * 0.5f,
                    lou_color);

                // Packages
                float32 backpack_x_offset = 0.4f;
                float32 backpack_y_offset = 0.2f;

                for (uint32 i = 0; i < gs->carried_packages; i++)
                {
                    if (i < 4)
                    {
                        draw_aligned_rectangle(
                            context, gs,
                            e->position.x - backpack_x_offset - i * 0.25f, e->position.y + backpack_y_offset,
                            package_height * 0.5f, package_width * 0.5f,
                            package_color);
                    }
                    else
                    {
                        draw_aligned_rectangle(
                            context, gs,
                            e->position.x - backpack_x_offset - 0.4f, e->position.y + 0.6f + (i - 4) * 0.25f + backpack_y_offset,
                            package_width * 0.5f, package_height * 0.5f,
                            package_color);
                    }
                }

                // Scanner
                {
                    draw_aligned_rectangle(
                        context, gs,
                        e->position.x - 0.4f, e->position.y + 0.5f,
                        0.175f, 0.05f,
                        porter_color);

                    draw_aligned_rectangle(
                        context, gs,
                        e->position.x - 0.5f, e->position.y + 1.f,
                        0.05f, 0.5f,
                        porter_color);
                    draw_aligned_rectangle(
                        context, gs,
                        e->position.x - 0.4f, e->position.y + 1.2f,
                        0.05f, 0.3f,
                        V4(1., 1., 1., 1.0f));
                }
            }
            break;

            case ENTITY_GROUND:
            {
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x, e->position.y,
                    e->width * 0.5f, e->height * 0.5f,
                    e->collided ? V4(1, 0, 0, 1) :
                    ground_color);
            }
            break;

            case ENTITY_POSTBOX:
            {
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x, e->position.y,
                    e->width * 0.5f, e->height * 0.5f,
                    porter_color);
            }
            break;

            case ENTITY_STONE:
            {
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x, e->position.y,
                    e->width * 0.5f, e->height * 0.5f,
                    stones_color);
            }
            break;

            case ENTITY_PACKAGE:
            {
                draw_aligned_rectangle(
                    context, gs,
                    e->position.x, e->position.y,
                    e->width * 0.5f, e->height * 0.5f,
                    is(e, ENTITY_ON_GROUND) ? V4(0.2, 0.9, 0.2, 1.0) :
                    package_color);
            }
            break;

            case ENTITY_INVALID:
                ASSERT_FAIL();
            break;
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
        draw_frame.color = V4(1,0,0,1);
        push_draw_screen_frame(context, draw_frame);
    }

    gs->near_exit_time -= dt;
    gs->blink_time -= dt;

    DEBUG_END_TIME_MEASUREMENT(update_and_render);
}

#if DLL_BUILD
#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <rs/resource_system.cpp>
#include <image/bmp.cpp>
#include <image/png.cpp>
#include <memory/crc.cpp>
#endif

#include <execution_context.cpp>
#include <ui/ui.cpp>
#if UI_EDITOR_ENABLED
#include <ui/ui_editor.cpp>
#endif

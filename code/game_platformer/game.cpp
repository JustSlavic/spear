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

#if OS_WINDOWS
#include <windows.h>
#define osOutputDebugString(MSG, ...) \
{  \
    char OutputBuffer_##__LINE__[256]; \
    sprintf(OutputBuffer_##__LINE__, MSG, __VA_ARGS__); \
    OutputDebugStringA(OutputBuffer_##__LINE__); \
} void(0)
#elif OS_LINUX
// #define osOutputDebugString(MSG, ...) printf(MSG, ##__VA_ARGS__)
#endif // OS_WINDOWS


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
#define BASE_FRICTION_COEFFICIENT         10.f


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


void draw_0(execution_context *context, game_state *gs, float32 offset_x, float32 offset_y)
{
    render_command::command_draw_mesh_with_color cmd;
    cmd.mesh_token = gs->zero_mesh;
    cmd.shader_token = gs->rectangle_shader;
    cmd.model =
        math::translated(V3(offset_x, offset_y, 0),
        math::scaled(V3(letter_width, letter_height, 1),
            math::matrix4::identity()));
    cmd.color = package_color;

    push_draw_mesh_with_color_command(context, cmd);
}

void draw_1(execution_context *context, game_state *gs, float32 offset_x, float32 offset_y)
{
    draw_aligned_rectangle(context, gs,
        offset_x,
        offset_y,
        letter_width * 0.25f, letter_height,
        package_color);
}

void draw_5(execution_context *context, game_state *gs, float32 offset_x, float32 offset_y)
{
    render_command::command_draw_mesh_with_color cmd;
    cmd.mesh_token = gs->five_mesh;
    cmd.shader_token = gs->rectangle_shader;
    cmd.model =
        math::translated(V3(offset_x, offset_y, 0),
        math::scaled(V3(letter_width, letter_height, 1),
            math::matrix4::identity()));
    cmd.color = package_color;

    push_draw_mesh_with_color_command(context, cmd);
}

void draw_10(execution_context *context, game_state *gs, float32 offset_x, float32 offset_y)
{
    render_command::command_draw_mesh_with_color cmd;
    cmd.mesh_token = gs->ten_mesh;
    cmd.shader_token = gs->rectangle_shader;
    cmd.model =
        math::translated(V3(offset_x, offset_y, 0),
        math::scaled(V3(letter_width, letter_height, 1),
            math::matrix4::identity()));
    cmd.color = package_color;

    push_draw_mesh_with_color_command(context, cmd);
}

void draw_50(execution_context *context, game_state *gs, float32 offset_x, float32 offset_y)
{
    render_command::command_draw_mesh_with_color cmd;
    cmd.mesh_token = gs->fifty_mesh;
    cmd.shader_token = gs->rectangle_shader;
    cmd.model =
        math::translated(V3(offset_x, offset_y, 0),
        math::scaled(V3(letter_width, letter_height, 1),
            math::matrix4::identity()));
    cmd.color = package_color;

    push_draw_mesh_with_color_command(context, cmd);
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

    gs->entities = (entity *) ALLOCATE_BUFFER_(&gs->game_allocator, sizeof(entity) * 20);
    gs->entities_capacity = 200000;
    // @note: let zero-indexed entity be 'null entity' representing lack of entity
    gs->entity_count = 1;

    auto ui_memory = ALLOCATE_BLOCK_(&gs->game_allocator, MEGABYTES(1));
    ui::initialize(gs, &gs->ui, ui_memory);

    auto group_1 = ui::make_group(&gs->ui, &gs->ui.root);
    auto shape_1 = ui::make_shape(&gs->ui, group_1);
    shape_1->position.xy = V2(500, 600);
    shape_1->rotation = 20.f;
    shape_1->color = V4(0.9, 0.4, 0.2, 1.0);

    auto hoverable_1 = ui::make_hoverable(&gs->ui, shape_1);
    hoverable_1->on_enter_internal = [](ui::system *s, ui::element *e)
    {
        e->color = V4(1, 0, 0, 1);
    };
    hoverable_1->on_leave_internal = [](ui::system *s, ui::element *e)
    {
        e->color = V4(0.9, 0.4, 0.2, 1.0);
    };

    ui::animate_ping_pong(&gs->ui, shape_1, ui::animation::POSITION_X, 60, 500, 600);
    ui::animate_ping_pong(&gs->ui, shape_1, ui::animation::COLOR_R, 30, 0, 1);
    ui::animate_ping_pong(&gs->ui, shape_1, ui::animation::COLOR_G, 30, 0, 1);
    ui::animate_ping_pong(&gs->ui, shape_1, ui::animation::COLOR_B, 30, 0, 1);

    ui::animate_ping_pong(&gs->ui, shape_1, ui::animation::WIDTH, 120, 100, 200);

    // auto group_1 = ui::make_group(&gs->ui, &gs->ui.root);
    // auto shape_1 = ui::make_shape(&gs->ui, group_1);
    // shape_1->position.xy = V2(200, 200);
    // shape_1->rotation = 20.f;
    // shape_1->color = V4(0.4, 0.7, 0.2, 1.0);

    // ui::give_name(&gs->ui, shape_1, STRID("Hello, Nikita!"));

    // auto hoverable_1 = ui::make_hoverable(&gs->ui, shape_1);
    // hoverable_1->on_enter_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(1, 0, 0, 1);
    // };
    // hoverable_1->on_leave_internal = [] (ui::system *s, ui::element *e)
    // {
    //     if (s->active != e)
    //         e->color = V4(0.4, 0.7, 0.2, 1.0);
    // };

    // auto clickable_1 = ui::make_clickable(&gs->ui, shape_1);
    // clickable_1->on_press_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(0, 1, 0, 1);
    // };
    // clickable_1->on_release_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(0.4, 0.7, 0.2, 1.0);
    //     if (s->hot)
    //         s->hot->hoverable->on_enter_internal(s, s->hot);
    // };

    // auto shape_2 = ui::make_shape(&gs->ui, group_1);
    // shape_2->position.xy = V2(300, 600);
    // shape_2->width = 300.f;
    // shape_2->height = 20.f;
    // shape_2->rotation = 45.f;
    // shape_2->color = V4(0.3, 0.6, 0.4, 1.0);

    // auto hoverable_2 = ui::make_hoverable(&gs->ui, shape_2);
    // hoverable_2->on_enter_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(1, 0, 0, 1);
    // };
    // hoverable_2->on_leave_internal = [] (ui::system *s, ui::element *e)
    // {
    //     if (s->active != e)
    //         e->color = V4(0.3, 0.6, 0.4, 1.0);
    // };

    // auto clickable_2 = ui::make_clickable(&gs->ui, shape_2);
    // clickable_2->on_press_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(0, 1, 0, 1);
    // };
    // clickable_2->on_release_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(0.3, 0.6, 0.4, 1.0);
    //     if (s->hot)
    //         s->hot->hoverable->on_enter_internal(s, s->hot);
    // };

    // auto shape_3 = ui::make_shape(&gs->ui, &gs->ui.root);
    // shape_3->position.xy = V2(400, 200);
    // shape_3->scale.xy = V2(5, 5);
    // shape_3->rotation = 70.f;
    // shape_3->color = V4(0.3, 0.3, 0.8, 1.0);

    // ui::animate_normal(&gs->ui, shape_3, ui::animation::POSITION_X, 60, 0, 100);

    // auto hoverable_3 = ui::make_hoverable(&gs->ui, shape_3);
    // hoverable_3->on_enter_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(1, 0, 0, 1);
    // };
    // hoverable_3->on_leave_internal = [] (ui::system *s, ui::element *e)
    // {
    //     if (e != s->active)
    //         e->color = V4(0.3, 0.3, 0.8, 1.0);
    // };

    // auto clickable_3 = ui::make_clickable(&gs->ui, shape_3);
    // clickable_3->on_press_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(0, 1, 0, 1);
    // };
    // clickable_3->on_release_internal = [] (ui::system *s, ui::element *e)
    // {
    //     e->color = V4(0.3, 0.3, 0.8, 1.0);
    //     if (s->hot)
    //         s->hot->hoverable->on_enter_internal(s, s->hot);
    // };

    // @note: This should be applied each frame after update phase, right?
    update_transforms(&gs->ui);

    gs->default_camera.position = V3(0, 0, 20);

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

    gs->ui.rectangle_mesh = gs->rectangle_mesh;
    gs->ui.rectangle_shader = gs->rectangle_shader;

    gs->blink_freq = 2.0f;

    // Mesh for letter V
    {
        float32 V_vbo[] = {
            -1.0f,  1.0f, 0.0f,
            -0.3f, -1.0f, 0.0f,
             0.3f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
             0.6f,  1.0f, 0.0f,
             0.0f, -0.7f, 0.0f,
            -0.6f,  1.0f, 0.0f,
        };

        auto _V_vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(V_vbo));
        memory::copy(_V_vbo.memory, V_vbo, sizeof(V_vbo));

        uint32 V_ibo[] = {
            0, 1, 6,
            1, 5, 6,
            5, 2, 3,
            5, 3, 4,
            1, 2, 5,
        };

        auto _V_ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(V_ibo));
        memory::copy(_V_ibo.memory, V_ibo, sizeof(V_ibo));

        gs->five_mesh = create_mesh_resource(&context->resource_storage, _V_vbo, _V_ibo, vbl);
    }

    // Mesh for letter L
    {
        float32 L_vbo[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f, -0.7f, 0.0f,
            -0.6f, -0.7f, 0.0f,
            -0.6f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
        };

        auto _L_vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(L_vbo));
        memory::copy(_L_vbo.memory, L_vbo, sizeof(L_vbo));

        uint32 L_ibo[] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
        };

        auto _L_ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(L_ibo));
        memory::copy(_L_ibo.memory, L_ibo, sizeof(L_ibo));

        gs->fifty_mesh = create_mesh_resource(&context->resource_storage, _L_vbo, _L_ibo, vbl);
    }

    // Mesh for letter O
    {
        float32 O_vbo[] = {
            -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -0.5f,  0.6f, 0.0f,
            -0.5f, -0.6f, 0.0f,
             0.5f, -0.6f, 0.0f,
             0.5f,  0.6f, 0.0f,
        };

        auto _O_vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(O_vbo));
        memory::copy(_O_vbo.memory, O_vbo, sizeof(O_vbo));

        uint32 O_ibo[] = {
            0, 1, 4,
            1, 5, 4,
            1, 2, 5,
            2, 6, 5,
            6, 2, 3,
            3, 7, 6,
            0, 4, 7,
            7, 3, 0,
        };

        auto _O_ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(O_ibo));
        memory::copy(_O_ibo.memory, O_ibo, sizeof(O_ibo));

        gs->zero_mesh = create_mesh_resource(&context->resource_storage, _O_vbo, _O_ibo, vbl);
    }

    // Mesh for letter X
    {
        float32 X_vbo[] = {
            -1.0f, -1.0f, 0.0f,
            -0.5f, -1.0f, 0.0f,
             0.0f, -0.3f, 0.0f,
             0.5f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             0.3f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
             0.5f,  1.0f, 0.0f,
             0.0f,  0.3f, 0.0f,
            -0.5f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            -0.3f,  0.0f, 0.0f,
        };

        auto _X_vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(X_vbo));
        memory::copy(_X_vbo.memory, X_vbo, sizeof(X_vbo));

        uint32 X_ibo[] = {
            0, 1, 11,
            1, 2, 11,
            2, 3, 4,
            4, 5, 2,
            2, 5, 11,
            11, 5, 8,
            5, 6, 7,
            5, 7, 8,
            8, 9, 10,
            10, 11, 8,
        };

        auto _X_ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(X_ibo));
        memory::copy(_X_ibo.memory, X_ibo, sizeof(X_ibo));

        gs->ten_mesh = create_mesh_resource(&context->resource_storage, _X_vbo, _X_ibo, vbl);
    }

    auto sam_ref = push_entity(gs);
    sam_ref.e->type     = ENTITY_SAM;
    sam_ref.e->flags    = ENTITY_INTERACTABLE | ENTITY_COLLIDABLE;
    sam_ref.e->position = V2(0, 5.4);
    sam_ref.e->velocity = V2(0);
    sam_ref.e->width    = 0.5f; // meters
    sam_ref.e->height   = 1.85f; // meters
    sam_ref.e->mass     = 80.0f; // kg

    gs->sam = sam_ref.e; // @IMPORTANT!

    auto ground = push_entity(gs);
    ground.e->type      = ENTITY_GROUND;
    ground.e->flags     = ENTITY_COLLIDABLE | ENTITY_STATIC;
    ground.e->position  = V2(0, -.5);
    ground.e->velocity  = V2(0);
    ground.e->width     = 200.f;
    ground.e->height    = 1.f;
    ground.e->mass      = 0.f;

    gs->ground = ground.e; // @IMPORTANT!

    // auto postbox = push_entity(gs);
    // postbox.e->type = ENTITY_POSTBOX;
    // postbox.e->position = V2(100, -3);
    // postbox.e->width = 1.f;
    // postbox.e->height = 2.f;
    // postbox.e->mass = 9000.f;

    // gs->postbox = postbox.e; // @IMPORTANT!

    // for (int i = 0; i < 50; i++)
    // {
    //     auto stone = push_entity(gs);
    //     stone.e->type = ENTITY_STONE;
    //     set(stone.e, ENTITY_STATIC);
    //     float32 x_mean = (i + 2) * 1.83f;
    //     stone.e->width = uniform_real(0.5f, 0.8f);
    //     stone.e->height = ((float32) uniform_int(2, 5)) * 0.1f;
    //     stone.e->position = V2(uniform_real(x_mean - 0.5f, x_mean + 0.5f), -4. + stone.e->height * 0.5f - uniform_real(0.1f, 0.3f));
    // }

    // for (int i = 0; i < 20; i++)
    // {
    //     auto package = push_entity(gs);
    //     package.e->type = ENTITY_PACKAGE;
    //     set(package.e, ENTITY_COLLIDABLE);
    //     float32 x_mean = (i + 3) * 3.f;
    //     package.e->position = V2(i + 1, 1); // V2(uniform_real(x_mean - 1.f, x_mean + 1.f), -2);
    //     package.e->width = package_width;
    //     package.e->height = package_height;
    // }
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

    // osOutputDebugString("mouse_p = {%d, %d}\n", input->mouse.x, input->mouse.y);

    DEBUG_BEGIN_TIME_MEASUREMENT(update_and_render);

    game_state *gs = (game_state *) game_memory.memory;
    sam_move move_data = {};

    if (get_press_count(input->keyboard[keyboard_device::esc]))
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

    if (get_hold_count(input->keyboard[keyboard_device::a]))
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
    if (get_hold_count(input->keyboard[keyboard_device::d]))
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

    if (get_press_count(input->keyboard[keyboard_device::space]))
    {
        move_data.acceleration.y += (PLAYER_BASE_JUMP_ACCELERATION / (0.1f * gs->carried_packages + 1)) / dt;
        move_data.jump = true;
    }

    // Setup camera
    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->default_camera.position;
        setup_camera.look_at_position = V3(gs->default_camera.position.x, gs->default_camera.position.y, 0);
        setup_camera.camera_up_direction = V3(0, 1, 0);

        push_setup_camera_command(context, setup_camera);
    }

    osOutputDebugString("GL_UNSIGNED_INT = %x\n", GL_UNSIGNED_INT);


    // Background
    // {
    //     render_command::command_draw_background draw_background;
    //     draw_background.mesh = gs->rectangle_mesh;
    //     draw_background.shader = gs->rectangle_shader;
    //     draw_background.color = sky_color;
    //     push_draw_background_command(context, draw_background);
    // }

    // if (ui::button(&gs->ui, STRID("Hello, Nikita!")))
    // {
    //     osOutputDebugString("HOVERED: TRUE\n");
    // }
    // else
    // {
    //     osOutputDebugString("HOVERED: FALSE\n");
    // }

    // // Coordinates
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
            for (int move = 0; move < 3; move++)
            {
                auto acceleration = V2(0);
                if (is_not(e, ENTITY_ON_GROUND))
                {
                    acceleration += gravity;
                }
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

                auto old_v = e->velocity;
                auto new_v = e->velocity + acceleration * dt_;

                auto friction = V2(0);
                if (is(e, ENTITY_ON_GROUND))
                {
                    // Friction with the ground
                    friction += (BASE_FRICTION_COEFFICIENT * (-new_v));
                    e->velocity.y = 0.f;
                }
                // @todo: should I consider air resitance?

                acceleration += friction;
                new_v = e->velocity + acceleration * dt_;
                e->velocity = new_v;

                auto old_p = e->position;
                auto new_p = e->position + old_v * dt_;

                auto full_distance = math::length(new_p - old_p);
                auto distance = full_distance;
                if (distance < EPSILON) break;

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

    // Score
    if (false)
    {
        float32 offset_x = gs->default_camera.position.x - 10.0f;
        float32 offset_y = gs->default_camera.position.y + 5.0f;

        if (gs->score == 0)
        {
            draw_0(context, gs, offset_x, offset_y);
        }
        else
        {
            uint32 score = gs->score;
            while(score > 0)
            {
                if (score >= 50)
                {
                    // draw L
                    draw_50(context, gs, offset_x, offset_y);
                    score -= 50;
                }
                else if (score >= 40)
                {
                    // draw XL
                    draw_10(context, gs, offset_x, offset_y);
                    offset_x += letter_width + spacing;
                    draw_50(context, gs, offset_x, offset_y);
                    score -= 40;
                }
                else if (score >= 10)
                {
                    // draw X
                    draw_10(context, gs, offset_x, offset_y);
                    score -= 10;
                }
                else if (score >= 9)
                {
                    // draw IX
                    draw_1(context, gs, offset_x, offset_y);
                    offset_x += letter_width + spacing;
                    draw_10(context, gs, offset_x, offset_y);
                    score -= 9;
                }
                else if (score >= 5)
                {
                    // draw V
                    draw_5(context, gs, offset_x, offset_y);
                    score -= 5;
                }
                else if (score >= 4)
                {
                    // draw IV
                    draw_1(context, gs, offset_x, offset_y);
                    offset_x += letter_width + spacing;
                    draw_5(context, gs, offset_x, offset_y);
                    score -= 4;
                }
                else
                {
                    // draw I
                    draw_1(context, gs, offset_x, offset_y);
                    score -= 1;
                }

                offset_x += letter_width + spacing;
            }
        }
    }

#if UI_EDITOR_ENABLED
    if (gs->ui_editor_enabled)
    {
        ui_update_editor(gs->ui_editor, gs->ui, input);

        // @note: reset hovered element in the UI, so in editor it would not stuck in hovered state.
        gs->ui->hovered_element = NULL;
    }
    else
    {
        ui_update_scene(gs->ui, input);
    }
    ui_draw_scene(gs->ui, Buffer);
    if (gs->ui_editor_enabled)
    {
        ui_draw_editor(gs->ui, gs->ui_editor, Buffer);
    }
#else // UI_EDITOR_ENABLED
    ui::update(&gs->ui, input);
    ui::draw(context, &gs->ui);
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


#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <rs/resource_system.cpp>
#include <execution_context.cpp>

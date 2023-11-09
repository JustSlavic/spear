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
    auto tex_resource = get_texture_resource(gs->button_push_1_texture);

    auto button = ui::make_group(gs->hud);
    ui::set_position(gs->hud, button, position);
    ui::make_hoverable(gs->hud, button, tex_resource->texture.width, tex_resource->texture.height);
    auto click_callbacks_4 = ui::make_clickable(gs->hud, button, tex_resource->texture.width, tex_resource->texture.height);
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

resource_token load_texture_resource_from_file(execution_context *context, char const *filename)
{
    resource_token result = {};

    memory_block file_content = context->debug_load_file(context->temporary_allocator, filename);
    if (file_content.memory != NULL)
    {
        auto bitmap = image::load_png(context->temporary_allocator, context->temporary_allocator, file_content);
        if (bitmap.pixels != NULL)
        {
            result = create_texture_resource(&context->rs, bitmap);
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

    gs->camera.position = V3(0, 0, -5);
    gs->camera.forward = V3(0, 0, 1);
    gs->camera.up = V3(0, 1, 0);

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

        gs->rectangle_mesh = create_mesh_resource(&context->rs, vbo, ibo, vbl);
        gs->rectangle_shader = create_shader_resource(&context->rs, STRID("rectangle.shader"));
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

        gs->rectangle_mesh_uv   = create_mesh_resource(&context->rs, vbo, ibo, vbl);
        gs->rectangle_shader_uv = create_shader_resource(&context->rs, STRID("rectangle_uv.shader"));
    }

    // 3D cube
    {
        float32 vbo_init[] = {
            // bottom square
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
            // top square
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
        };

        uint32 ibo_init[] = {
            0, 1, 2,
            2, 3, 0,

            0, 3, 7,
            7, 4, 0,

            1, 5, 6,
            6, 2, 1,

            3, 2, 6,
            6, 7, 3,

            1, 0, 4,
            4, 5, 1,

            5, 4, 7,
            7, 6, 5,
        };

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        auto vbo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(vbo_init));
        memory__copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(ibo_init));
        memory__copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->cube_mesh = create_mesh_resource(&context->rs, vbo, ibo, vbl);
    }

    // UI
    {
        auto ui_memory = ALLOCATE_BUFFER_(gs->game_allocator, MEGABYTES(1));
        gs->hud = ui::initialize(ui_memory);
#if UI_EDITOR_ENABLED
        gs->ui_editor = ui::initialize_editor(gs->game_allocator);
#endif // UI_EDITOR_ENABLED
    }
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

    auto right_direction = to_vector3(outer(gs->camera.forward, gs->camera.up));
    if (get_hold_count(input->keyboard[KB_A]))
    {
        gs->camera.position -= normalized(right_direction) * dt;
    }
    if (get_hold_count(input->keyboard[KB_D]))
    {
        gs->camera.position += normalized(right_direction) * dt;
    }
    if (get_hold_count(input->keyboard[KB_W]))
    {
        gs->camera.position += normalized(gs->camera.up) * dt;
    }
    if (get_hold_count(input->keyboard[KB_S]))
    {
        gs->camera.position -= normalized(gs->camera.up) * dt;
    }

    // Setup camera
    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera.position;
        setup_camera.look_at_position = gs->camera.position + gs->camera.forward;
        setup_camera.camera_up_direction = gs->camera.up;

        push_setup_camera_command(context, setup_camera);
    }

    // Background
    // {
    //     render_command::command_draw_background draw_background;
    //     draw_background.mesh = gs->rectangle_mesh;
    //     draw_background.shader = gs->rectangle_shader;
    //     draw_background.color = sky_color;
    //     push_draw_background_command(context, draw_background);
    // }

    // Draw ground
    {
        render_command::command_draw_mesh_with_color draw_mesh;

        draw_mesh.mesh_token = gs->cube_mesh;
        draw_mesh.shader_token = gs->rectangle_shader;
        draw_mesh.model = matrix4__identity();
        draw_mesh.color = V4(0.3, 0.8, 0.4, 1);

        push_draw_mesh_with_color_command(context, draw_mesh);
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

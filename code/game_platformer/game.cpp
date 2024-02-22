#include "game.hpp"
#include <game_interface.hpp>


// @todo:
// + packages
// + pick up packages (backpack)
// + on hit stone loose packages
// + on hit stone teleport onto this stone with the signature blinking
// + sent packages via postbox
// + draw the score number as roman numerals


#define STRID(CSTR) string_id::from(&context->strid_storage, CSTR)


#include <integer.h>
#include <math/rectangle2.hpp>
#include <vector2.hpp>

#include <collision.hpp>
#include <image/png.hpp>

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

    memory_buffer file_content = context->debug_load_file(context->temporary_allocator, filename);
    if (file_content.data != NULL)
    {
        auto bitmap = image::load_png(context->temporary_allocator, context->temporary_allocator, file_content);
        if (bitmap.pixels != NULL)
        {
            result = create_texture_resource(&context->rs, bitmap);
        }
    }

    return result;
}


INITIALIZE_MEMORY_FUNCTION(execution_context *context, memory_buffer game_memory)
{
    using namespace math;

    srand((unsigned int)time(NULL));

    global_debug_measurements = context->debug_measurements;

    ASSERT(sizeof(game_state) < game_memory.size);
    ASSERT(context->debug_load_file);

    memory_allocator game_allocator = memory_allocator::make_arena(game_memory);
    game_state *gs = game_allocator.allocate<game_state>();
    gs->game_allocator = game_allocator;
    context->game_state = gs;

    gs->camera.position = V3(0, -5.5, 5);
    gs->camera.forward = normalized(V3(0, 1, -1));
    gs->camera.up = normalized(V3(0, 1, 1));

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

        auto vbo = context->temporary_allocator.allocate_copy(vbo_init, sizeof(vbo_init));
        auto ibo = context->temporary_allocator.allocate_copy(ibo_init, sizeof(ibo_init));

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

        auto vbo = context->temporary_allocator.allocate_copy(vbo_init, sizeof(vbo_init));
        auto ibo = context->temporary_allocator.allocate_copy(ibo_init, sizeof(ibo_init));

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

        auto vbo = context->temporary_allocator.allocate_copy(vbo_init, sizeof(vbo_init));
        auto ibo = context->temporary_allocator.allocate_copy(ibo_init, sizeof(ibo_init));

        gs->cube_mesh = create_mesh_resource(&context->rs, vbo, ibo, vbl);
    }

    // UI
    {
        auto ui_memory = gs->game_allocator.allocate_buffer(MEGABYTES(1));
        gs->hud = ui::initialize(ui_memory);
#if UI_EDITOR_ENABLED
        gs->ui_editor = ui::initialize_editor(gs->game_allocator);
#endif // UI_EDITOR_ENABLED
    }
}


UPDATE_AND_RENDER_FUNCTION(execution_context *context, memory_buffer game_memory, input_state input)
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

    if (get_hold_count(input->keyboard[KB_A]))
    {
        gs->camera.position -= V3(1, 0, 0) * dt;
    }
    if (get_hold_count(input->keyboard[KB_D]))
    {
        gs->camera.position += V3(1, 0, 0) * dt;
    }
    if (get_hold_count(input->keyboard[KB_W]))
    {
        gs->camera.position += V3(0, 1, 0) * dt;
    }
    if (get_hold_count(input->keyboard[KB_S]))
    {
        gs->camera.position -= V3(0, 1, 0) * dt;
    }
    if (get_hold_count(input->keyboard[KB_R]))
    {
        gs->camera.position += V3(0, 0, 1) * dt;
    }
    if (get_hold_count(input->keyboard[KB_F]))
    {
        gs->camera.position -= V3(0, 0, 1) * dt;
    }

    // Background
    {
        render_command::command_draw_background draw_background;
        draw_background.mesh = gs->rectangle_mesh;
        draw_background.shader = gs->rectangle_shader;
        draw_background.color = sky_color;
        push_draw_background_command(context, draw_background);
    }

    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera.position;
        setup_camera.look_at_position = gs->camera.position + gs->camera.forward;
        setup_camera.camera_up_direction = gs->camera.up;

        push_setup_camera_command(context, setup_camera);
    }


    // Throw ray to intersect XY-plane
    bool32 intersection_found = false;
    vector3 intersection = V3(0);

    // auto mouse_x = clamp(((float32) input->mouse.x + context->letterbox_width * 0.5f) * 2.f / context->letterbox_width - 2.f, -1.f, 1.f);
    // auto mouse_y = clamp(-((float32) input->mouse.y + context->letterbox_height * 0.5f) * 2.f / context->letterbox_height + 2.f, -1.f, 1.f);

// X = (x + w*0.5)*2/w - 2

    // osOutputDebugString("mouse: (x, y) = (%4.2f, %4.2f)\n", mouse_x, mouse_y);

    // {
    //     auto ratio = ((float32) context->letterbox_width / context->letterbox_height);

    //     auto NEAR_CLIP_DIST = 0.05f;
    //     auto clip_width = NEAR_CLIP_DIST;
    //     auto clip_height = NEAR_CLIP_DIST / ratio;

    //     auto clip_mouse_x = mouse_x * clip_width * 0.5f;
    //     auto clip_mouse_y = mouse_y * clip_height * 0.5f;

    //     auto camera_right = normalized(cross(gs->camera.forward, gs->camera.up));

    //     auto near_plane_center = gs->camera.position + gs->camera.forward * NEAR_CLIP_DIST;
    //     auto near_plane_point  = near_plane_center +
    //                              camera_right * clip_mouse_x +
    //                              gs->camera.up * clip_mouse_y;

    //     osOutputDebugString("clip plane: (%f, %f)\n", clip_width, clip_height);

    //     osOutputDebugString("clip mouse: (%f, %f)\n", clip_mouse_x, clip_mouse_y);

    //     {
    //         render_command::command_draw_mesh_with_color draw_mesh;
    //         draw_mesh.mesh_token = gs->rectangle_mesh;
    //         draw_mesh.shader_token = gs->rectangle_shader;
    //         draw_mesh.model = matrix4::identity()
    //                         * matrix4::translate(near_plane_center + gs->camera.forward * 0.001f)
    //                         * matrix4::rotate_x(to_radians(45.f))
    //                         * matrix4::scale(0.0005f, 0.0005f, 0.005f);
    //         draw_mesh.color = V4(1, 0, 1, 1);
    //         push_draw_mesh_with_color_command(context, draw_mesh);
    //     }

    //     {
    //         render_command::command_draw_mesh_with_color draw_mesh;
    //         draw_mesh.mesh_token = gs->rectangle_mesh;
    //         draw_mesh.shader_token = gs->rectangle_shader;
    //         draw_mesh.model = matrix4::identity()
    //                         * matrix4::translate(near_plane_point)
    //                         * matrix4::rotate_x(to_radians(45.f))
    //                         * matrix4::scale(0.0002f, 0.0002f, 0.002f);
    //         draw_mesh.color = V4(1, 1, 1, 1);
    //         push_draw_mesh_with_color_command(context, draw_mesh);
    //     }

    //     auto ray_start = gs->camera.position;
    //     auto ray_direction = normalized(near_plane_point - gs->camera.position);

    //     // Line-plane intersection

    //     // Plane is z=0

    //     // let n be
    //     auto n = V3(0, 0, 1);

    //     // let p0 be a known point on a plane
    //     auto p0 = V3(0);

    //     // thus dot(p - p0, n) = 0

    //     // let point on the line be p = l0 + l * d,
    //     // where l is direction, l0 is known point on the line

    //     // thus
    //     // dot(l0 + l*d - p0, n) = 0
    //     // dot(ld, n) + dot(l0 - p0, n) = 0
    //     // d * dot(l, n) + dot(l0 - p0, n) = 0
    //     // d = dot(p0 - l0, n) / dot(l, n)

    //     auto l = ray_direction;
    //     auto l0 = ray_start;

    //     auto denom = inner(l, n);
    //     if (absolute(denom) < EPSILON)
    //     {
    //         intersection_found = false;
    //     }
    //     else
    //     {
    //         intersection_found = true;
    //         float32 d = inner(p0 - l0, n) / denom;

    //         intersection = ray_start + ray_direction * d;

    //         osOutputDebugString("intersection: (%4.2f, %4.2f)\n", intersection.x, intersection.y);
    //     }
    // }

    // Draw ground
    for (int32 x = -2; x <= 2; x++)
    {
        for (int32 y = -2; y <= 2; y++)
        {
            render_command::command_draw_mesh_with_color draw_mesh;

            draw_mesh.mesh_token = gs->rectangle_mesh;
            draw_mesh.shader_token = gs->rectangle_shader;
            draw_mesh.model = matrix4::identity()
                            * matrix4::translate_x((float32) x)
                            * matrix4::translate_y((float32) y)
                            * matrix4::scale(0.48f, 0.48f, 0.1f);
            draw_mesh.color = V4(0.4 * (x * 0.25f + 0.25f), 0.8 * (y * 0.25f + 0.5f), 0.4, 1);

            // if (truncate_to_int32(intersection.x - 0.5f) == x &&
            //     truncate_to_int32(intersection.y - 0.5f) == y)
            // {
            //    draw_mesh.color = V4(1, 0, 0, 1);
            // }

            push_draw_mesh_with_color_command(context, draw_mesh);
        }
    }

    {
        render_command::command_draw_mesh_with_color draw_mesh;
        draw_mesh.mesh_token = gs->rectangle_mesh;
        draw_mesh.shader_token = gs->rectangle_shader;
        draw_mesh.model = matrix4::identity()
                        * matrix4::translate_x((float32) intersection.x)
                        * matrix4::translate_y((float32) intersection.y)
                        * matrix4::translate_z(0.01f)
                        * matrix4::scale(0.05f, 0.05f, 0.1f);
        draw_mesh.color = V4(1, 0, 0, 1);
        push_draw_mesh_with_color_command(context, draw_mesh);
    }

// #if UI_EDITOR_ENABLED
//     if (gs->ui_editor_enabled)
//     {
//         ui::update_editor(gs->hud, gs->ui_editor, input);
//         // Reset s-> hot, active, pressed
//     }
//     else
//     {
//         ui::update(gs->hud, input);
//     }
//     ui::render(context, gs->hud);
//     if (gs->ui_editor_enabled)
//     {
//         ui::render_editor(context, gs->hud, gs->ui_editor);

//         {
//             render_command::command_draw_screen_frame draw_frame;
//             draw_frame.model = matrix4::identity();
//             draw_frame.view = matrix4::identity();
//             draw_frame.projection = matrix4::identity();
//             draw_frame.color = V4(0,0,0,1);
//             push_draw_screen_frame(context, draw_frame);
//         }
//     }
// #else // UI_EDITOR_ENABLED
//     ui::update(gs->hud, input);
//     ui::render(context, gs->hud);
// #endif // UI_EDITOR_ENABLED

    if (gs->near_exit_time > 0)
    {
        render_command::command_draw_screen_frame draw_frame;
        draw_frame.model = matrix4::identity();
        draw_frame.view = matrix4::identity();
        draw_frame.projection = matrix4::identity();
        draw_frame.color = V4(1,0,0,1);
        push_draw_screen_frame(context, draw_frame);

        gs->near_exit_time -= dt;
        if (gs->near_exit_time < 0)
            gs->near_exit_time = 0;
    }
}

#if DLL_BUILD
#include <memory_allocator.cpp>
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

#include <base.h>

#include <array.hpp>
#include <math/integer.hpp>
#include <g2.hpp>
#include <g3.hpp>
#include <math/rectangle2.hpp>
#include <rs/resource_system.hpp>
#include <ui/ui.hpp>
#if UI_EDITOR_ENABLED
#include <ui/ui_editor.hpp>
#endif


#if DEBUG
struct debug_time_measurement;
extern debug_time_measurement *global_debug_measurements;
extern uint32 global_debug_call_depth;

#define DEBUG_BEGIN_TIME_MEASUREMENT(NAME) \
    uint64 debug_begin_time_measurement_##NAME##__ = DEBUG_CYCLE_COUNT(); \
    (void) 0

#define DEBUG_END_TIME_MEASUREMENT(NAME) \
    uint64 debug_end_time_measurement_##NAME##__ = DEBUG_CYCLE_COUNT(); \
    add_measurement(global_debug_measurements + DEBUG_TIME_SLOT_##NAME, debug_end_time_measurement_##NAME##__ - debug_begin_time_measurement_##NAME##__); \
    (void) 0
#else
#define DEBUG_BEGIN_TIME_MEASUREMENT(NAME)
#define DEBUG_END_TIME_MEASUREMENT(NAME)
#endif


struct game__camera {
    vector3 position;
    vector3 forward;
    vector3 up;
};

struct sam_move
{
    vector2 acceleration;
    vector2 velocity;
    bool32 jump;
};

struct game_state
{
    memory_allocator game_allocator;
    float32 near_exit_time;

    game__camera camera;

    resource_token rectangle_mesh;
    resource_token circle_mesh;
    resource_token rectangle_shader;

    resource_token rectangle_mesh_uv;
    resource_token rectangle_shader_uv;

    ui::system *hud;
#if UI_EDITOR_ENABLED
    ui::editor *ui_editor;
    bool32 ui_editor_enabled;
#endif
};




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

#include <image/png.hpp>

#include <g2.hpp>
#include <g201.hpp>

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



void pga2__draw_line(execution_context *context, game_state *gs, pga2::line line, vector4 color, float32 z = 0.f)
{
    // Find points intersecting the right and left borders of the screen:
    // line: ax + by + c = 0
    // let x =  1 => y = -c/b - a/b
    // let x = -1 => y = -c/b + a/b

    auto y1 = -line.c / line.b - line.a / line.b;
    auto y2 = -line.c / line.b + line.a / line.b;

    // let x = 0 => y = -c/b

    // The rotation around z is going to be such:
    auto n = normalized(V2(line.a, line.b));
    auto angle = atan2f(-n.x, n.y);
    auto d = line.c / math::square_root(line.a * line.a + line.b * line.b);

    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->rectangle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate_x(math::near_zero(line.a) ? 0 : -0.2f * n.x * d)
                    * matrix4__translate_y(math::near_zero(line.b) ? 0 : -0.2f * n.y * d)
                    * matrix4__translate_z(z)
                    * matrix4__rotate_z(angle)
                    * matrix4__scale(10.f, 0.004f, 1)
                    ;
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}


void pga2__draw_point(execution_context *context, game_state *gs, pga2::point p, vector4 color)
{
    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->circle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate_x(0.2f * p.x / p.c)
                    * matrix4__translate_y(0.2f * p.y / p.c)
                    * matrix4__scale(.03f, .03f, 1)
                    ;
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
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

    gs->camera.position = V3(0, 0, 5);
    gs->camera.forward = normalized(V3(0, 0, -1));
    gs->camera.up = normalized(V3(0, 1, 0));

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

    // Init circle mesh
    {
        constexpr uint32 n_gram = 15;

        float32 vbo_init[(n_gram + 1)*3];
        vbo_init[0] = 0.f;
        vbo_init[1] = 0.f;
        vbo_init[2] = 0.f;

        auto d_angle = 2.0f * math::pi / n_gram;
        auto angle = 0.f;

        for (int i = 3; i < ARRAY_COUNT(vbo_init);)
        {
            auto v = V2(math::cos(angle), math::sin(angle));
            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 0.f;

            angle += d_angle;
        }

        uint32 ibo_init[3*n_gram];
        uint32 running_index = 1;
        for (int i = 0; i < ARRAY_COUNT(ibo_init);)
        {
            ibo_init[i++] = 0;
            ibo_init[i++] = running_index++;
            ibo_init[i++] = running_index > n_gram ? 1 : running_index;
        }

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        auto vbo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(vbo_init));
        memory__copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(ibo_init));
        memory__copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->circle_mesh = create_mesh_resource(&context->rs, vbo, ibo, vbl);
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

    // Setup camera
    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera.position;
        setup_camera.look_at_position = gs->camera.position + gs->camera.forward;
        setup_camera.camera_up_direction = gs->camera.up;

        push_setup_camera_command(context, setup_camera);
    }

    int32 grid_size = 30;
    for (int32 i = -grid_size; i < grid_size; i++)
    {
        if (i == 0) continue;

        pga2::line l = {};
        l.a = (float32) 1;
        l.c = (float32) i;
        pga2__draw_line(context, gs, l, V4(0.2, 0.2, 0.2, 1), -0.02f);
    }

    for (int32 i = -grid_size; i < grid_size; i++)
    {
        if (i == 0) continue;

        pga2::line l = {};
        l.b = (float32) 1;
        l.c = (float32) i;
        pga2__draw_line(context, gs, l, V4(0.2, 0.2, 0.2, 1), -0.02f);
    }

    {
        pga2::line line_Ox = {};
        line_Ox.b = 1;
        pga2__draw_line(context, gs, line_Ox, V4(0.5, 0, 0, 1), -0.01f);

        pga2::line line_Oy = {};
        line_Oy.a = 1;
        pga2__draw_line(context, gs, line_Oy, V4(0.1, 0.5, 0.1, 1), -0.01f);
    }

    {
        pga2::line l1 = {};
        l1.a = -23.f;
        l1.b = 1.f;
        l1.c = 10.f;
        pga2__draw_line(context, gs, l1, V4(0.8, 0.5, 0.2, 1));

        pga2::line l2 = {};
        l2.a = -1.f;
        l2.b = 7.f;
        l2.c = 1.f;
        pga2__draw_line(context, gs, l2, V4(0.2, 0.8, 0.7, 1));

        pga2::line l12 = l1 + l2;
        pga2__draw_line(context, gs, l12, V4(0.6, 0.3, 0.8, 1));

        pga2::point p = meet(l1, l2);
        pga2__draw_point(context, gs, p, V4(1, 0, 0, 1));
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

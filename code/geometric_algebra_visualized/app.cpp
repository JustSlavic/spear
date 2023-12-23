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

    float32 camera_rotation_t;

    resource_token rectangle_mesh;
    resource_token circle_mesh;
    resource_token cilinder_mesh;
    resource_token triangle_mesh;
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

#include <pga2.hpp>
#include <pga3.hpp>

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

#define console_print osOutputDebugString


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

GLOBAL vector4 background_color = V4(15.0 / 255.0, 20.0 / 255.0, 20.0 / 255.0, 1.0);

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


#define GLOBAL_SCALE  0.2f


void pga2__draw_line(execution_context *context, game_state *gs, pga2::line line, vector4 color, float32 z);
void pga2__draw_point(execution_context *context, game_state *gs, pga2::point p, vector4 color);
void pga2__draw_triangle(execution_context *context, game_state *gs, vector2 p, float32 angle, vector4 color);
void pga2__draw_segment(execution_context *context, game_state *gs, vector2 p, vector2 q, vector4 color);
void pga2__draw_vector(execution_context *context, game_state *gs, vector2 p, vector4 color);
// void pga2__draw_vector(execution_context *context, game_state *gs, pga2::point q, pga2::point p, vector4 color);


void pga2__draw_line(execution_context *context, game_state *gs, pga2::line line, vector4 color, float32 z = 0.f)
{
    // line: ax + by + c = 0
    // let x =  1 => y = -c/b - a/b
    // let x = -1 => y = -c/b + a/b

    auto y1 = -line.c / line.b - line.a / line.b;
    auto y2 = -line.c / line.b + line.a / line.b;

    // let x = 0 => y = -c/b

    // The rotation around z is going to be such:
    auto angle = atan2f(-line.a, line.b);
    auto d = line.c / math::square_root(line.a * line.a + line.b * line.b);

    auto n = normalized(V2(line.a, line.b));

    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->rectangle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate_x(-GLOBAL_SCALE * n.x * d)
                    * matrix4__translate_y(-GLOBAL_SCALE * n.y * d)
                    * matrix4__translate_z(z)
                    * matrix4__rotate_z(angle)
                    * matrix4__scale(10.f, 0.004f, 1);
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}

void pga2__draw_line_and_dual(execution_context *context, game_state *gs, pga2::line line, vector4 color, float32 z = 0.f)
{
    pga2__draw_line(context, gs, line, color, z);
    pga2__draw_point(context, gs, dual(line), 0.3f * color);
}

void pga2__draw_point(execution_context *context, game_state *gs, pga2::point p, vector4 color)
{
    if (near_zero(p.w, 0.005f))
    {
        pga2__draw_vector(context, gs, p.vector, color);
        return;
    }

    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->circle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate_x(GLOBAL_SCALE * p.x / p.w)
                    * matrix4__translate_y(GLOBAL_SCALE * p.y / p.w)
                    * matrix4__scale(.03f, .03f, 1);
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}

void pga2__draw_point_and_dual(execution_context *context, game_state *gs, pga2::point p, vector4 color)
{
    pga2__draw_point(context, gs, p, color);
    pga2__draw_line(context, gs, dual(p), 0.3f * color);
}

void pga2__draw_segment(execution_context *context, game_state *gs, vector2 start, vector2 end, vector4 color)
{
    auto c = 0.5f * (start + end);

    auto d = end - start;
    auto angle = atan2f(d.y, d.x);

    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->rectangle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate_x(GLOBAL_SCALE * c.x)
                    * matrix4__translate_y(GLOBAL_SCALE * c.y)
                    * matrix4__rotate_z(angle)
                    * matrix4__scale(0.5f * GLOBAL_SCALE * length(d), 0.5f * GLOBAL_SCALE * 0.1f, 1.f);
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}

void pga2__draw_triangle(execution_context *context, game_state *gs, vector2 p, float32 angle, vector4 color)
{
    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->triangle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate_x(GLOBAL_SCALE * p.x)
                    * matrix4__translate_y(GLOBAL_SCALE * p.y)
                    * matrix4__rotate_z(angle)
                    * matrix4__scale(GLOBAL_SCALE * 0.5f, GLOBAL_SCALE * 0.4f * 0.5f, 1.f);
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}

void pga2__draw_vector(execution_context *context, game_state *gs, vector2 p, vector4 color)
{
    auto q = p - 0.5f * normalized(p);
    pga2__draw_segment(context, gs, V2(0), q, color);
    pga2__draw_triangle(context, gs, q, atan2f(p.y, p.x), color);
}

void pga2__draw_vector(execution_context *context, game_state *gs, vector2 r, pga2::point p, vector4 color)
{
    auto q = to_vector2(p) - 0.5f * normalized(V2(p.x, p.y));
    pga2__draw_segment(context, gs, r, r + to_vector2(p), color);
    pga2__draw_triangle(context, gs, r + q, atan2f(p.y, p.x), color);
}


void pga3__draw_line(execution_context *context, game_state *gs, vector4 color)
{
    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->cilinder_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__scale(0.02f * GLOBAL_SCALE, 0.02f * GLOBAL_SCALE, 100 * GLOBAL_SCALE)
                    * matrix4__translate_z(-0.5f)
                    ;
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}

void pga3__draw_segment(execution_context *context, game_state *gs, vector3 a, vector3 b, vector4 color)
{
    auto n = (b - a);

    auto q = normalized(V3(0, 0, 1) * bisector(V3(0, 0, 1), n));
    auto R = to_matrix4(q);

    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->cilinder_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate(GLOBAL_SCALE * a)
                    * R
                    * matrix4__scale(0.02f * GLOBAL_SCALE, 0.02f * GLOBAL_SCALE, length(b - a) * GLOBAL_SCALE)
                    ;
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);
}

void pga3__draw_plane(execution_context *context, game_state *gs, plane3 p, vector4 color)
{
    auto q = normalized(V3(0, 0, 1) * bisector(V3(0, 0, 1), p.normal));
    auto R = to_matrix4(q);

    auto d = -normalized(p.normal) * math::absolute(p.w) / norm(p.normal);

    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->rectangle_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate(d * GLOBAL_SCALE)
                    * R
                    * matrix4__scale(GLOBAL_SCALE * norm(p.normal), GLOBAL_SCALE * norm(p.normal), GLOBAL_SCALE)
                    ;
    draw_mesh.color = color;
    push_draw_mesh_with_color_command(context, draw_mesh);

    pga3__draw_segment(context, gs, d, d + p.normal, color * 0.5f);
}

void pga3__draw_point(execution_context *context, game_state *gs, vector3 p, vector4 color)
{
    render_command::command_draw_mesh_with_color draw_mesh;
    draw_mesh.mesh_token = gs->cilinder_mesh;
    draw_mesh.shader_token = gs->rectangle_shader;
    draw_mesh.model = matrix4__identity()
                    * matrix4__translate(p*GLOBAL_SCALE)
                    * matrix4__scale(GLOBAL_SCALE*0.1f)
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

    // Init triangle mesh
    {
        float32 vbo_init[] = {
            0.0f,  1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            1.0f,  0.0f, 0.0f,
        };

        uint32 ibo_init[] = {
            0, 1, 2,
        };

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        auto vbo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(vbo_init));
        memory__copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(ibo_init));
        memory__copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->triangle_mesh = create_mesh_resource(&context->rs, vbo, ibo, vbl);
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

    // Init cilinder mesh
    {
        constexpr uint32 n_gram = 20;

        float32 vbo_init[2*3*n_gram];
        uint32 ibo_init[2*3*(n_gram - 2) + 2*3*n_gram];

        auto d_angle = 2.0f * math::pi / n_gram;
        auto angle = 0.f;
        int i = 0;
        int j = 0;
        int vertex_index = 0;

        {
            auto v = V2(math::cos(angle), math::sin(angle));
            angle += d_angle;

            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 0.f;

            vertex_index += 1;
        }

        {
            auto v = V2(math::cos(angle), math::sin(angle));
            angle += d_angle;

            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 0.f;

            vertex_index += 1;
        }

        for (int k = vertex_index; k < n_gram; k++)
        {
            auto v = V2(math::cos(angle), math::sin(angle));
            angle += d_angle;

            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 0.f;

            ibo_init[j++] = 0;
            ibo_init[j++] = vertex_index - 1;
            ibo_init[j++] = vertex_index;

            vertex_index += 1;
        }

        angle = 0.f;
        {
            auto v = V2(math::cos(angle), math::sin(angle));
            angle += d_angle;

            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 1.f;

            vertex_index += 1;
        }

        {
            auto v = V2(math::cos(angle), math::sin(angle));
            angle += d_angle;

            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 1.f;

            vertex_index += 1;
        }

        for (int k = vertex_index; k < 2*n_gram; k++)
        {
            auto v = V2(math::cos(angle), math::sin(angle));
            angle += d_angle;

            vbo_init[i++] = v.x;
            vbo_init[i++] = v.y;
            vbo_init[i++] = 1.f;

            ibo_init[j++] = n_gram;
            ibo_init[j++] = vertex_index - 1;
            ibo_init[j++] = vertex_index;

            vertex_index += 1;
        }

        vertex_index = 1;
        while (vertex_index < n_gram)
        {
            ibo_init[j++] = vertex_index;
            ibo_init[j++] = vertex_index - 1;
            ibo_init[j++] = vertex_index + n_gram;

            ibo_init[j++] = vertex_index + n_gram;
            ibo_init[j++] = vertex_index - 1;
            ibo_init[j++] = vertex_index - 1 + n_gram;

            vertex_index += 1;
        }

        gfx::vertex_buffer_layout vbl = {};
        gfx::push_layout_element(&vbl, 3);

        auto vbo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(vbo_init));
        memory__copy(vbo.memory, vbo_init, sizeof(vbo_init));

        auto ibo = ALLOCATE_BUFFER_(context->temporary_allocator, sizeof(ibo_init));
        memory__copy(ibo.memory, ibo_init, sizeof(ibo_init));

        gs->cilinder_mesh = create_mesh_resource(&context->rs, vbo, ibo, vbl);
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

#define PGA3
    auto red    = V4(0.9, 0.1, 0.1, 1.f);
    auto orange = V4(0.8, 0.4, 0.2, 1.f);
    auto green  = V4(0.4, 0.8, 0.2, 1.f);
    auto blue   = V4(0.4, 0.4, 1.0, 1.f);
    auto purple = V4(0.8, 0.2, 0.8, 1.f);

    auto x = (float32)  input->mouse.x / context->letterbox_width  - 0.5f;
    auto y = (float32) -input->mouse.y / context->letterbox_height + 0.5f;

#ifdef PGA2
    // Setup camera
    {
        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera.position;
        setup_camera.look_at_position = gs->camera.position + gs->camera.forward;
        setup_camera.camera_up_direction = gs->camera.up;

        push_setup_camera_command(context, setup_camera);
    }
    // Background
    {
        render_command::command_draw_background draw_background;
        draw_background.mesh = gs->rectangle_mesh;
        draw_background.shader = gs->rectangle_shader;
        draw_background.color = background_color;
        push_draw_background_command(context, draw_background);
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

    pga2__draw_line(context, gs, pga2::Ox, V4(0.5, 0, 0, 1), -0.01f);
    pga2__draw_line(context, gs, pga2::Oy, V4(0.1, 0.5, 0.1, 1), -0.01f);

#define PGA2_CASE 9
    // 1: Lines and their dual points, and vice versa
    // 2: Draw a line through two points
    // 3: Perpendicular and parallel lines to another line through a point
    // 4: Reflection of a line across another line
    // 5: Reflect a point across a line
    // 6: Rotate a line around a point
    // 7: Rotate point around another point
    // 8: Duals of lines that go though the origin are _vectors_ (points at infinity)

    osOutputDebugString("(%f, %f)\n", x, y);

#if (PGA2_CASE == 1)
    {
        auto p1 = pga2::make_point(-1, 1);
        auto p2 = pga2::make_point(2, 2);

        auto l1 = dual(p1);
        auto l2 = dual(p2);

        if (x > 0.f)
        {
            pga2__draw_point_and_dual(context, gs, p1, green);
            pga2__draw_point_and_dual(context, gs, p2, blue);
        }
        else
        {
            pga2__draw_line_and_dual(context, gs, l1, green);
            pga2__draw_line_and_dual(context, gs, l2, blue);
        }
    }
#endif

#if (PGA2_CASE == 2)
    // Draw a line through two points
    {
        auto p1 = pga2::make_point(-5.f, 3.f);
        auto p2 = pga2::make_point(3.f, 1.f);
        auto result_line = join(p1, p2);

        pga2__draw_point_and_dual(context, gs, p1, orange);
        pga2__draw_point_and_dual(context, gs, p2, blue);
        pga2__draw_line_and_dual(context, gs, result_line, green);
    }
#endif

#if (PGA2_CASE == 3)
    // Perpendicular and parallel lines to another line through a point
    {
        auto p1 = pga2::make_point(2.f, 2.f);
        auto l1 = pga2::make_line(-1.f, 2.f, 3.f);

        pga2__draw_point(context, gs, p1, red);
        pga2__draw_line(context, gs, l1, orange);

        auto l_perpendicular = inner(l1, p1);
        pga2__draw_line(context, gs, l_perpendicular, green);

        auto l_parallel = inner(l_perpendicular, p1);
        pga2__draw_line(context, gs, l_parallel, blue);
    }
#endif

#if (PGA2_CASE == 4)
    // Reflection of a line across another line
    {
        auto a = pga2::make_line(-1.f, 3.f, -2.f);
        auto l1 = pga2::make_line(-1.f, 1.f, -2.f);
        auto l2 = to_line(a * l1 * a);

        pga2__draw_line(context, gs, a, blue);
        pga2__draw_line(context, gs, l1, green);
        pga2__draw_line(context, gs, l2, green);
    }
#endif

#if (PGA2_CASE == 5)
    // Reflect a point across a line
    {
        auto a = pga2::make_line(-1.f, 3.f, -2.f);
        pga2__draw_line(context, gs, a, blue);

        auto l1 = pga2::make_line(1, 2, -5);
        auto l2 = pga2::make_line(-3, 1, 0);
        pga2__draw_line(context, gs, l1, blue);
        pga2__draw_line(context, gs, l2, blue);

        auto pt1 = outer(l1, l2);
        pga2__draw_point(context, gs, pt1, orange);

        auto l1_r = to_line(a * l1 * a);
        auto l2_r = to_line(a * l2 * a);

        pga2__draw_line(context, gs, l1_r, orange);
        pga2__draw_line(context, gs, l2_r, orange);

        auto pt3 = to_point(a * pt1 * a);
        pga2__draw_point(context, gs, pt3, red);

        auto l_perpendicular = inner(a, pt1);
        pga2__draw_line(context, gs, l_perpendicular, green);
    }
#endif

#if (PGA2_CASE == 6)
    // Rotate a line around a point
    {
        auto p = pga2::make_point(3.0f, 3.0f);
        auto l1 = pga2::make_line(3.f, 2.f, -2.f);

        auto alpha = 6.f * x;

        auto p1 = p + V2(0, 1);
        auto p2 = p + math::cos(alpha) * V2(1, 0) + math::sin(alpha) * V2(0, 1);

        auto a = join(p, p1);
        auto b = join(p, p2);

        pga2__draw_line(context, gs, a, blue);
        pga2__draw_line(context, gs, b, blue);

        auto motor = a * b;

        auto l2 = to_line(conjugated(motor) * l1 * motor);

        pga2__draw_point(context, gs, p, red);
        pga2__draw_line(context, gs, l1, red);
        pga2__draw_line(context, gs, l2, red);
    }
#endif

#if (PGA2_CASE == 7)
    // Rotate point around another point
    {
        auto p = pga2::make_point(3.0f, 3.0f);
        pga2__draw_point(context, gs, p, red);

        auto alpha = 6.f * x;

        auto p1 = p + V2(0, 1);
        auto p2 = p + math::cos(alpha) * V2(1, 0) + math::sin(alpha) * V2(0, 1);

        auto a = join(p, p1);
        auto b = join(p, p2);

        pga2__draw_line(context, gs, a, blue);
        pga2__draw_line(context, gs, b, blue);

        auto motor = a * b;

        auto pt1 = pga2::make_point(-1.f, 1.f);
        auto pt2 = to_point(conjugated(motor) * pt1 * motor);

        pga2__draw_point(context, gs, pt1, green);
        pga2__draw_point(context, gs, pt2, green);
    }
#endif

#if (PGA2_CASE == 8)
    // Duals of lines that go though the origin are _vectors_ (points at infinity)
    {
        auto a = pga2::make_line(1, x, x);
        auto p = dual(a);

        pga2__draw_line(context, gs, a, orange);
        pga2__draw_point(context, gs, p, orange);
    }
#endif

#if (PGA2_CASE == 9)
    // Parallel translation of the line could be done with the motor
    {
        auto a = pga2::make_line(1.f, 5.f, -1.f);
        auto b = a + V2(3, 2);

        pga2__draw_line_and_dual(context, gs, a, orange);
        pga2__draw_line_and_dual(context, gs, b, orange);

        auto motor = a * b;

        auto l1 = pga2::make_line(1.1f, 1.f, 0.f);
        auto l2 = to_line(conjugated(motor) * l1 * motor);

        pga2__draw_line_and_dual(context, gs, l1, blue);
        pga2__draw_line_and_dual(context, gs, l2, blue);
    }
#endif

#endif // #ifdef PGA2


#ifdef PGA3
    // Setup camera
    {
        auto R = 2.f;
        auto H = 2.f;
        auto c = math::cos(gs->camera_rotation_t);
        auto s = math::sin(gs->camera_rotation_t);

        gs->camera_rotation_t += 0.25f * dt;
        if (gs->camera_rotation_t > 2 * math::pi)
        {
            gs->camera_rotation_t -= 2 * math::pi;
        }

        auto r = V3(R * c, R * s, H);
        auto v = V3(-s, c, 0);

        gs->camera.position = r;
        gs->camera.forward = normalized(-gs->camera.position);
        gs->camera.up = normalized(cross(r, v));

        render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera.position;
        setup_camera.look_at_position = gs->camera.position + gs->camera.forward;
        setup_camera.camera_up_direction = gs->camera.up;

        push_setup_camera_command(context, setup_camera);
    }

    int32 grid_size = 30;
    for (int32 i = -grid_size; i < grid_size; i++)
    {
        pga2::line l = {};
        l.a = (float32) 1;
        l.c = (float32) i;
        pga2__draw_line(context, gs, l, V4(0.2, 0.2, 0.2, 1), -0.002f);
    }

    for (int32 i = -grid_size; i < grid_size; i++)
    {
        pga2::line l = {};
        l.b = (float32) 1;
        l.c = (float32) i;
        pga2__draw_line(context, gs, l, V4(0.2, 0.2, 0.2, 1), -0.002f);
    }

    pga3__draw_segment(context, gs, V3(0), V3(10, 0, 0), red);
    pga3__draw_segment(context, gs, V3(0), V3(0, 10, 0), green);
    pga3__draw_segment(context, gs, V3(0), V3(0, 0, 10), blue);


#define PGA3_CASE 2


#if PGA3_CASE == 1
    {
        auto p = make_plane3(x, 1, 0, y);
        pga3__draw_plane(context, gs, p, orange);
    }
#endif

#if PGA3_CASE == 2
    {
        auto p1 = make_plane3(0, 2, 2, 0);
        pga3__draw_plane(context, gs, p1, blue * 0.9f);

        auto p2 = make_plane3(1, 0, 1, 1);
        pga3__draw_plane(context, gs, p2, orange * 0.9f);

        auto l = outer(p1, p2);

        auto d = norm(to_vector3(l.moment)) / norm(l.direction);
        auto dv = d * normalized(cross(l.direction, to_vector3(l.moment)));

        // pga3__draw_segment(context, gs, V3(0), dv, green);
        // pga3__draw_segment(context, gs, -3.f*l.direction + dv, 3.f*l.direction + dv, red);

        pga3__draw_segment(context, gs, V3(-10, -9, 9), V3(10, 11, -11), red);

        console_print("d: %f direction = (%f, %f, %f); moment = (yz=%f, zx=%f, xy=%f)\n",
            d,
            l.direction.x, l.direction.y, l.direction.z,
            l.moment.yz, l.moment.zx, l.moment.xy);

        // auto i = inner(l.moment, l.direction);
        // osOutputDebugString("%f %f %f\n", i.x, i.y, i.z);

        // vector3 pl1 = get_point_on_line(l);
        // osOutputDebugString("%f, %f, %f\n", pl1.x, pl1.y, pl1.z);

        // pga3__draw_point(context, gs, pl1, red);
    }
#endif

#if PGA3_CASE == 3

#endif

#if PGA3_CASE == 4

#endif

#if PGA3_CASE == 5

#endif

#if PGA3_CASE == 6

#endif

#if PGA3_CASE == 7

#endif

#if PGA3_CASE == 8

#endif

#endif // #ifdef PGA3



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

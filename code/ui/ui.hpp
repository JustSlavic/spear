#ifndef UI_HPP
#define UI_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <execution_context.hpp>
#include <math/vector2.hpp>
#include <math/vector4.hpp>
#include <math/transform.hpp>
#include <array.hpp>
#include <string_id.hpp>
#include <input.hpp>

#if OS_WINDOWS
#include <windows.h>
#include <stdio.h>
#define osOutputDebugString(MSG, ...) \
{  \
    char OutputBuffer_##__LINE__[256]; \
    sprintf(OutputBuffer_##__LINE__, MSG, __VA_ARGS__); \
    OutputDebugStringA(OutputBuffer_##__LINE__); \
} void(0)
#elif OS_LINUX
#define osOutputDebugString(MSG, ...) printf(MSG, ##__VA_ARGS__)
#endif // OS_WINDOWS


struct game_state;


namespace ui {


struct system;
struct element;
struct hover_behaviour;
struct click_behaviour;


typedef void callback(system *, element *);
void callback_stub(system *, element *) {}


enum class array_of
{
    // 0 is "none", so ui_id with all bits set
    // to 0 represents "null" reference
    none = 0,
    groups,
    shapes,
    hoverables,
    clickables,
};


struct ui_id
{

    // 8 bits array_of
    // 24 bits index in that array
    uint32 id;
};


ui_id make_ui_id(array_of a, uint32 index)
{
    ui_id result;
    result.id = ((((uint32) a) << 24) | (index & 0x00ffffff));
    return result;
}


array_of which_array(ui_id id)
{
    auto result = (array_of) (id.id >> 24);
    return result;
}


uint32 get_index(ui_id id)
{
    uint32 result = (id.id & 0x00ffffff);
    return result;
}


struct element
{
    ui_id id;

    // These are for transform properties
    math::vector3 position;
    math::vector3 scale;
    float32 rotation;

    // @note: width, height, and color are only for shapes
    float32 width;
    float32 height;
    math::vector4 color;

    // Parent-child relations
    element *parent;
    static_array<ui_id, 10> children; // @todo: make it grow if needed? bucket_array?
    uint32 order_index;

    // Optional slots
    hover_behaviour *hoverable;
    click_behaviour *clickable;

    // Cache
    math::transform transform; // transform * vector_in_child => vector_in_parent
    math::transform transform_to_root; // Cached matrix to transform vectors from child space to root's
};

struct hover_behaviour
{
    ui_id id;
    bool32 hovered;

    element *owner;
    math::rectangle2 hover_area;

    callback *on_enter;
    callback *on_leave;

    callback *on_enter_internal;
    callback *on_leave_internal;
};

struct click_behaviour
{
    element *owner;
    math::rectangle2 click_area;

    callback *on_press;
    callback *on_release;

    callback *on_press_internal;
    callback *on_release_internal;
};


struct animation
{
    enum property
    {
        POSITION_X,
        POSITION_Y,
        ROTATION,
        WIDTH,
        HEIGHT,
        COLOR_R,
        COLOR_G,
        COLOR_B,
        COLOR_A,

        PROPERTY_COUNT
    };

    enum function
    {
        LINEAR,
    };

    enum kind
    {
        NORMAL, // Goes forward and stops
        NORMAL_LOOP, // Goes forward and loops from the start
        PING_PONG, // Goes forward and then backwards closing the loop
        // UI_ANIMATION_REVERSE,
        // UI_ANIMATION_ALTERNATE,
        // UI_ANIMATION_ALTERNATE_REVERSE,
    };

    enum direction : int32
    {
        FORWARD  =  1,
        BACKWARD = -1,
    };

    ui_id element_id;

    property p;
    kind type;

    float32 start_value;
    float32 final_value;

    // Assume begin time be 0s
    float32 duration_seconds; // the same as end time
    float32 current_time;
    direction is_forward;
};

struct system
{
    memory::allocator ui_allocator;
    game_state *gs;

    // @todo: expand it to grow in needed?
    static_array<element, 10> groups;
    static_array<element, 10> shapes;
    static_array<hover_behaviour, 10> hoverables;
    static_array<click_behaviour, 10> clickables;
    static_array<animation, 10> animations;

    element root;
    element *hot;    // The element is about to be interacted with (mouse over or focused)
    element *active; // The element is being interacted with right now (mouse is down)

    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;

    string_id hash_table_strids[32]; // Keys
    ui_id     hash_table_ids[32];    // Values
};

void initialize(game_state *gs, system *sys, memory_block ui_memory)
{
    memory::set(sys, 0, sizeof(system));
    memory::initialize_memory_arena(&sys->ui_allocator, ui_memory);

    sys->gs = gs;

    sys->root.transform = math::transform::identity();
    sys->root.transform_to_root = math::transform::identity();
}

void give_name(system *s, element *e, string_id strid)
{
    for (usize offset = 0; offset < ARRAY_COUNT(s->hash_table_strids); offset++)
    {
        uint32 index = (strid.id + offset) % ARRAY_COUNT(s->hash_table_strids);

        if (s->hash_table_strids[index].id == 0)
        {
            s->hash_table_strids[index] = strid;
            s->hash_table_ids[index] = e->id;
            break;
        }
    }
}

element *get_element_by_id(system *s, ui_id id)
{
    element *result = NULL;

    uint32 index = get_index(id);
    switch (which_array(id))
    {
        case array_of::groups:
        {
            result = s->groups.data + index;
        }
        break;

        case array_of::shapes:
        {
            result = s->shapes.data + index;
        }
        break;

        case array_of::none:
        case array_of::hoverables:
        case array_of::clickables:
        break;
    }

    return result;
}

element *get_element_by_name(system *s, string_id strid)
{
    ui_id id = {};

    for (usize offset = 0; offset < ARRAY_COUNT(s->hash_table_strids); offset++)
    {
        uint32 index = (strid.id + offset) % ARRAY_COUNT(s->hash_table_strids);

        if (s->hash_table_strids[index] == strid)
        {
            id = s->hash_table_ids[index];
            break;
        }
        else if (s->hash_table_strids[index].id == 0)
        {
            break;
        }
    }

    element *result = get_element_by_id(s, id);
    return result;
}

element *make_group(system *sys, element *parent)
{
    ui_id id = make_ui_id(array_of::groups, truncate_to_uint32(sys->groups.size));

    element *result = sys->groups.push();
    memory::set(result, 0, sizeof(element));
    result->id = id;
    result->scale = V3(1);
    result->transform = math::transform::identity();
    result->transform_to_root = math::transform::identity();

    result->parent = parent;
    parent->children.push(id);

    return result;
}

element *make_shape(system *sys, element *parent)
{
    ui_id id = make_ui_id(array_of::shapes, truncate_to_uint32(sys->shapes.size));

    element *result = sys->shapes.push();
    memory::set(result, 0, sizeof(element));
    result->id = id;
    result->width = 100.f;
    result->height = 100.f;
    result->scale = V3(1);
    result->transform = math::transform::identity();
    result->transform_to_root = math::transform::identity();

    result->parent = parent;
    parent->children.push(id);

    return result;
}

hover_behaviour *make_hoverable(system *sys, element *e)
{
    hover_behaviour *result = NULL;
    if (e->hoverable == NULL)
    {
        ui_id id = make_ui_id(array_of::hoverables, truncate_to_uint32(sys->hoverables.size));

        result = sys->hoverables.push();
        result->id = id;
        result->hovered = false;
        result->owner = e;
        result->hover_area = math::rectangle2::from_center_size(V2(0), 100, 100);
        result->on_enter = callback_stub;
        result->on_leave = callback_stub;
        result->on_enter_internal = callback_stub;
        result->on_leave_internal = callback_stub;

        e->hoverable = result;
    }
    return result;
}

click_behaviour *make_clickable(system *sys, element *e)
{
    click_behaviour *result = NULL;
    if (e->clickable == NULL)
    {
        result = sys->clickables.push();
        result->owner = e;
        result->click_area = math::rectangle2::from_center_size(V2(0), 100, 100);
        result->on_press = callback_stub;
        result->on_release = callback_stub;
        result->on_press_internal = callback_stub;
        result->on_release_internal = callback_stub;

        e->clickable = result;
    }
    return result;
}

void update_transform(element *e)
{
    auto transform =
        math::rotated_z(math::to_radians(e->rotation),
        math::scaled(e->scale,
        math::translated(e->position,
        math::transform::identity())));
    e->transform = transform;
}

void update_transforms(system *sys)
{
    // @todo: make BFS here (where to allocate queue? scratchpad memory?)
    for (usize i = 0; i < sys->groups.size; i++)
    {
        element *e = sys->groups.data + i;
        update_transform(e);
        e->transform_to_root = e->parent->transform_to_root * e->transform;
    }
    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        update_transform(e);
        e->transform_to_root = e->parent->transform_to_root * e->transform;
    }
}

void update_order_index(system *sys, element *e, uint32& order_index)
{
    e->order_index = order_index++;
    for (usize child_index = 0; child_index < e->children.size; child_index++)
    {
        ui_id child_id = e->children.data[child_index];
        element *child = get_element_by_id(sys, child_id);
        update_order_index(sys, child, order_index);
    }
}

void make_cold(system *sys, element *e)
{
    sys->hot->hoverable->on_leave_internal(sys, sys->hot);
    sys->hot->hoverable->on_leave(sys, sys->hot);
    sys->hot->hoverable->hovered = false;
    sys->hot = NULL;
}

void make_hot(system *sys, element *e)
{
    sys->hot = e;
    sys->hot->hoverable->hovered = true;
    sys->hot->hoverable->on_enter(sys, sys->hot);
    sys->hot->hoverable->on_enter_internal(sys, sys->hot);
}

void update_animations(system *sys, input *inp)
{
    for (uint32 i = 0; i < sys->animations.size;)
    {
        animation& a = sys->animations[i];
        element *e = get_element_by_id(sys, a.element_id);
        a.current_time += ((float32) a.is_forward) * inp->dt;

        if (a.current_time > a.duration_seconds)
        {
            if (a.type == animation::NORMAL)
            {

            }
            if (a.type == animation::NORMAL_LOOP)
            {
                a.current_time -= a.duration_seconds;
            }
            if (a.type == animation::PING_PONG)
            {
                a.current_time = a.duration_seconds;
                a.is_forward = animation::BACKWARD;
            }
        }
        if (a.current_time < 0)
        {
            if (a.type == animation::NORMAL)
            {
                a.current_time = 0.f;
            }
            if (a.type == animation::NORMAL_LOOP)
            {
                a.current_time = 0.f;
            }
            if (a.type == animation::PING_PONG)
            {
                a.current_time = 0.f;
                a.is_forward = animation::FORWARD;
            }
        }

        // @note: assumes only LINEAR function for now
        // @todo: generalize this function stuff
        float32 new_value = math::lerp(a.start_value, a.final_value, a.current_time / a.duration_seconds);

        switch (a.p)
        {
            case animation::POSITION_X: e->position.x = new_value;
                break;
            case animation::POSITION_Y: e->position.y = new_value;
                break;
            case animation::ROTATION: e->rotation = new_value;
                break;
            case animation::WIDTH: e->width = new_value;
                break;
            case animation::HEIGHT: e->height = new_value;
                break;
            case animation::COLOR_R: e->color.r = new_value;
                break;
            case animation::COLOR_G: e->color.g = new_value;
                break;
            case animation::COLOR_B: e->color.b = new_value;
                break;
            case animation::COLOR_A: e->color.a = new_value;
                break;

            case animation::PROPERTY_COUNT:
                break;
        }

        if (a.current_time > a.duration_seconds)
        {
            sys->animations.erase_not_sorted(i);
        }
        else
        {
            i += 1;
        }
    }
}

void update(system *sys, input *inp)
{
    auto mouse_position = V3(inp->mouse.x, inp->mouse.y, 0);

    uint32 order_index = 0;
    update_order_index(sys, &sys->root, order_index);

    update_animations(sys, inp);

    element *hovered = NULL;

    for (usize i = 0; i < sys->hoverables.size; i++)
    {
        hover_behaviour *hoverable = sys->hoverables.data + i;

        auto inverse_transform = inverse(hoverable->owner->transform_to_root);
        auto mouse_position_local = inverse_transform * mouse_position;

        if (math::is_inside(hoverable->hover_area, mouse_position_local.xy))
        {
            if (hovered == NULL)
            {
                hovered = hoverable->owner;
            }
            else if (hoverable->owner->order_index < hovered->order_index)
            {
                hovered = hoverable->owner;
            }
        }
    }

    if (hovered)
    {
        if (sys->active == NULL)
        {
            // I found element under mouse, and no element is active! That's good, I can set myself as hot.
            if (sys->hot != NULL)
            {
                // There's something hot already, check if it's what I found.
                if (sys->hot == hovered)
                {
                    // The element I found under the mouse is exactly what I have hot. I will do nothing then.
                }
                else
                {
                    // This is new element! Remove hot from old one and make hot a new one!
                    make_cold(sys, sys->hot);
                    make_hot(sys, hovered);
                }
            }
            else
            {
                // There's nothing hot yet, let's make our element hot.
                make_hot(sys, hovered);
            }
        }
        else
        {
            // When this happens?
        }
    }
    else
    {
        if (sys->hot)
        {
            // I didn't find anything under the mouse, but I have a hot element? Make it cold again.
            make_cold(sys, sys->hot);
        }
    }

    if (get_press_count(inp->mouse[mouse_device::lmb]))
    {
        sys->active = sys->hot;
        if (sys->active)
        {
            if (sys->active->clickable)
            {
                if (sys->active == sys->hot)
                {
                    sys->active->clickable->on_press_internal(sys, sys->active);
                    sys->active->clickable->on_press(sys, sys->active);
                }
            }
        }
    }

    if (get_release_count(inp->mouse[mouse_device::lmb]))
    {
        if (sys->active)
        {
            if (sys->active->clickable)
            {
                if (sys->active == sys->hot)
                {
                    sys->active->clickable->on_release(sys, sys->active);
                }
                sys->active->clickable->on_release_internal(sys, sys->active);
            }
            sys->active = NULL;
        }
    }

    // @note: This should be applied each frame after update phase, right?
    update_transforms(sys);
}

void draw(execution_context *context, system *sys)
{
    auto projection =
        math::translated(V3(-1, 1, 0),
        math::scaled(V3(2.0/context->letterbox_width, -2.0/context->letterbox_height, 1),
        math::matrix4::identity()));

    for (usize i = sys->shapes.size - 1; i < sys->shapes.size; i--)
    {
        element *e = sys->shapes.data + i;

        auto model =
            math::scaled(V3(0.5f * e->width, 0.5f * e->height, 1),
            math::to_matrix4(e->transform_to_root));

        render_command::command_draw_ui command_draw_ui;
        command_draw_ui.mesh_token = sys->rectangle_mesh;
        command_draw_ui.shader_token = sys->rectangle_shader;
        command_draw_ui.model = math::transposed(model); // @todo: remove transpose after I make all matrix4 be m * v instead of v * m as for now
        command_draw_ui.view = math::matrix4::identity();
        command_draw_ui.projection = projection;
        command_draw_ui.color = e->color;

        render_command cmd;
        cmd.type = render_command::command_type::draw_ui;
        cmd.draw_ui = command_draw_ui;

        push_render_command(context, cmd);
    }
}


void animate(system *s, element *e, animation::property property, animation::kind type, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animation a;
    a.element_id = e->id;
    a.p = property;
    a.type = type;

    a.start_value = start_value;
    a.final_value = final_value;

    // @note: assume all animations are 60 frames per second
    a.duration_seconds = (float32) length_in_frames / 60.f;
    a.current_time = 0.f;
    a.is_forward = animation::FORWARD;

    s->animations.push(a);
}


void animate_normal(system *s, element *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::NORMAL, length_in_frames, start_value, final_value);
}


void animate_normal_loop(system *s, element *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::NORMAL_LOOP, length_in_frames, start_value, final_value);
}


void animate_ping_pong(system *s, element *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::PING_PONG, length_in_frames, start_value, final_value);
}


bool32 button(system *s, string_id id)
{
    element *e = get_element_by_name(s, id);
    bool32 result = (e && e->hoverable && e->hoverable->hovered);
    return result;
}


} // namespace ui

#endif // UI_HPP

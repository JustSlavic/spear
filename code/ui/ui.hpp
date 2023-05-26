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
#define osOutputDebugString(MSG, ...) printf(MSG, __VA_ARGS__)
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
    static_array<element *, 10> children; // @todo: make it grow if needed? bucket_array?
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

struct system
{
    memory::allocator ui_allocator;
    game_state *gs;

    // @todo: expand it to grow in needed?
    static_array<element, 10> groups;
    static_array<element, 10> shapes;
    static_array<hover_behaviour, 10> hoverables;
    static_array<click_behaviour, 10> clickables;

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
    for (int offset = 0; offset < ARRAY_COUNT(s->hash_table_strids); offset++)
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
    }

    return result;
}

element *get_element_by_name(system *s, string_id strid)
{
    ui_id id = {};

    for (int offset = 0; offset < ARRAY_COUNT(s->hash_table_strids); offset++)
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
    parent->children.push(result);

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
    parent->children.push(result);

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

void update_order_index(element *e, uint32& order_index)
{
    e->order_index = order_index++;
    for (usize child_index = 0; child_index < e->children.size; child_index++)
    {
        element *child = e->children.data[child_index];
        update_order_index(child, order_index);
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

void update(system *sys, input *inp)
{
    auto mouse_position = V3(inp->mouse.x, inp->mouse.y, 0);

    uint32 order_index = 0;
    update_order_index(&sys->root, order_index);

    hover_behaviour *hovered = NULL;

    for (usize i = 0; i < sys->hoverables.size; i++)
    {
        hover_behaviour *hoverable = sys->hoverables.data + i;

        auto inverse_transform = inverse(hoverable->owner->transform_to_root);
        auto mouse_position_local = inverse_transform * mouse_position;

        if (math::is_inside(hoverable->hover_area, mouse_position_local.xy))
        {
            if (hovered == NULL)
            {
                hovered = hoverable;
            }
            else if (hoverable->owner->order_index < hovered->owner->order_index)
            {
                hovered = hoverable;
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
                if (sys->hot == hovered->owner)
                {
                    // The element I found under the mouse is exactly what I have hot. I will do nothing then.
                }
                else
                {
                    // This is new element! Remove hot from old one and make hot a new one!
                    make_cold(sys, sys->hot);
                    make_hot(sys, hovered->owner);
                }
            }
            else
            {
                // There's nothing hot yet, let's make our element hot.
                make_hot(sys, hovered->owner);
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


bool32 button(system *s, string_id id)
{
    element *e = get_element_by_name(s, id);
    bool32 result = (e && e->hoverable && e->hoverable->hovered);
    return result;
}


} // namespace ui

#endif // UI_HPP

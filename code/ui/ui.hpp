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


namespace ui {


struct system;
struct group;
struct shape;
struct hover_behaviour;
struct click_behaviour;


typedef void callback(system *, group *);
void callback_stub(system *, group *) {}


enum class array_of : uint8
{
    // 0 is "none", so handle with all bits set
    // to 0 represents "null" reference
    NONE = 0,
    GROUPS,
    SHAPES,
};


struct handle
{

    // 8 bits array_of
    // 24 bits index in that array
    uint32 id;

    operator bool32 () { return (id != 0); }
};

bool32 operator == (handle a, handle b)
{
    bool32 result = (a.id == b.id);
    return result;
}

bool32 operator != (handle a, handle b)
{
    bool32 result = (a.id != b.id);
    return result;
}

handle null_handle()
{
    handle result = {};
    return result;
}

handle make_handle(array_of a, uint32 index)
{
    handle result;
    result.id = ((((uint32) a) << 24) | (index & 0x00ffffff));
    return result;
}

array_of which_array(handle id)
{
    auto result = (array_of) (id.id >> 24);
    return result;
}

uint32 get_index(handle id)
{
    uint32 result = (id.id & 0x00ffffff);
    return result;
}


struct group
{
    handle id;
    uint32 order_index;

    // These are for transform properties
    math::vector3 position;
    math::vector3 scale;
    float32 rotation;
    // Cache
    math::transform transform; // transform * vector_in_child => vector_in_parent
    math::transform transform_to_root; // Cached matrix to transform vectors from child space to root's

    // Parent-child relations
    group *parent;
    static_array<handle, 10> children; // @todo: make it grow if needed? bucket_array?

    // Optional slots
    hover_behaviour *hoverable;
    click_behaviour *clickable;
};


struct shape
{
    handle id;
    uint32 order_index;

    // These are for transform properties
    math::vector3 position;
    math::vector3 scale;
    float32 rotation;
    // Cache
    math::transform transform; // transform * vector_in_child => vector_in_parent
    math::transform transform_to_root; // Cached matrix to transform vectors from child space to root's

    // @note: width, height, and color are only for shapes
    float32 width;
    float32 height;
    math::vector4 color;

    // Parent-child relations
    group *parent;
};


struct hover_behaviour
{
    bool32 hovered;

    group *owner;
    math::rectangle2 hover_area;

    callback *on_enter;
    callback *on_leave;

    callback *on_enter_internal;
    callback *on_leave_internal;
};

struct click_behaviour
{
    group *owner;
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
        NORMAL,      // Goes forward and stops
        NORMAL_LOOP, // Goes forward and loops from the start
        PING_PONG,   // Goes forward and then backwards closing the loop
    };

    enum direction : int32
    {
        FORWARD  =  1,
        BACKWARD = -1,
    };

    handle element_id;

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

    // @todo: expand it to grow in needed?
    static_array<group, 10> groups;
    static_array<shape, 10> shapes;
    static_array<hover_behaviour, 10> hoverables;
    static_array<click_behaviour, 10> clickables;
    static_array<animation, 10> animations;

    group root;
    handle hot;     // The element is about to be interacted with (mouse over or focused)
    handle active;  // The element is being interacted with right now (mouse is down)
    handle pressed; // The element is clicked, and action is performed (one frame only!)

    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;

    uint64 hash_table_keys[32];
    handle hash_table_values[32];
};

void initialize(system *s, memory_block ui_memory)
{
    memory::set(s, 0, sizeof(system));
    memory::initialize_memory_arena(&s->ui_allocator, ui_memory);

    s->root.transform = math::transform::identity();
    s->root.transform_to_root = math::transform::identity();
}

void push_to_hash_table(system *s, uint64 key, handle value)
{
    for (int offset = 0; offset < ARRAY_COUNT(s->hash_table_keys); offset++)
    {
        usize index = (key + offset) % ARRAY_COUNT(s->hash_table_keys);
        if (s->hash_table_values[index].id == 0) // @note: a valid handle cannot be 0
        {
            s->hash_table_keys[index] = key;
            s->hash_table_values[index] = value;
            break;
        }
    }
}

handle get_handle_from_hash_table(system *s, uint64 key)
{
    handle result = {};

    for (int offset = 0; offset < ARRAY_COUNT(s->hash_table_keys); offset++)
    {
        uint32 index = (key + offset) % ARRAY_COUNT(s->hash_table_keys);
        if (s->hash_table_keys[index] == key)
        {
            result = s->hash_table_values[index];
            break; // Successfully found a value
        }
        if (s->hash_table_values[index].id == 0)
        {
            break; // Didn't find a value
        }
    }

    return result;
}


group *get_group(system *s, handle h)
{
    group *result = NULL;

    uint32 index = get_index(h);
    if (which_array(h) == array_of::GROUPS)
    {
        result = s->groups.data + index;
    }

    return result;
}


shape *get_shape(system *s, handle h)
{
    shape *result = NULL;

    uint32 index = get_index(h);
    if (which_array(h) == array_of::SHAPES)
    {
        result = s->shapes.data + index;
    }

    return result;
}


group *make_group(system *s, group *parent)
{
    handle id = make_handle(array_of::GROUPS, truncate_to_uint32(s->groups.size));

    group *result = s->groups.push();
    memory::set(result, 0, sizeof(group));
    result->id = id;
    result->scale = V3(1);
    result->transform = math::transform::identity();
    result->transform_to_root = math::transform::identity();

    result->parent = parent;
    parent->children.push(id);

    return result;
}

shape *make_shape(system *s, group *parent)
{
    handle id = make_handle(array_of::SHAPES, truncate_to_uint32(s->shapes.size));

    shape *result = s->shapes.push();
    memory::set(result, 0, sizeof(shape));
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

hover_behaviour *make_hoverable(system *s, group *e)
{
    hover_behaviour *result = NULL;
    if (e->hoverable == NULL)
    {
        result = s->hoverables.push();
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

click_behaviour *make_clickable(system *s, group *e)
{
    click_behaviour *result = NULL;
    if (e->clickable == NULL)
    {
        result = s->clickables.push();
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

void update_transform_for_group(group *e)
{
    auto transform =
        math::rotated_z(math::to_radians(e->rotation),
        math::scaled(e->scale,
        math::translated(e->position,
        math::transform::identity())));
    e->transform = transform;
}


void update_transform_for_shape(shape *e)
{
    auto transform =
        math::rotated_z(math::to_radians(e->rotation),
        math::scaled(e->scale,
        math::translated(e->position,
        math::transform::identity())));
    e->transform = transform;
}

void update_transforms(system *s)
{
    // @todo: make BFS here (where to allocate queue? scratchpad memory?)
    for (usize i = 0; i < s->groups.size; i++)
    {
        group *e = s->groups.data + i;
        update_transform_for_group(e);
        e->transform_to_root = e->parent->transform_to_root * e->transform;
    }
    for (usize i = 0; i < s->shapes.size; i++)
    {
        shape *e = s->shapes.data + i;
        update_transform_for_shape(e);
        e->transform_to_root = e->parent->transform_to_root * e->transform;
    }
}

void update_order_index_for_group(system *s, group *e, uint32& order_index)
{
    e->order_index = order_index++;
    for (usize child_index = 0; child_index < e->children.size; child_index++)
    {
        handle child_handle = e->children.data[child_index];
        switch (which_array(child_handle))
        {
            case array_of::GROUPS:
            {
                group *child = get_group(s, child_handle);
                update_order_index_for_group(s, child, order_index);
            }
            break;

            case array_of::SHAPES:
            {
                shape *child = get_shape(s, child_handle);
                child->order_index = order_index++;
            }
            break;
        }
    }
}

void make_cold(system *s, handle h)
{
    auto *e = get_group(s, h);
    e->hoverable->on_leave_internal(s, e);
    e->hoverable->on_leave(s, e);
    e->hoverable->hovered = false;
    s->hot = null_handle();
}

void make_hot(system *s, group *e)
{
    s->hot = e->id;
    e->hoverable->hovered = true;
    e->hoverable->on_enter(s, e);
    e->hoverable->on_enter_internal(s, e);
}

void update_animations(system *s, input *inp)
{
    for (uint32 i = 0; i < s->animations.size;)
    {
        animation& a = s->animations[i];
        a.current_time += ((float32) a.is_forward) * inp->dt;

        if (a.current_time > a.duration_seconds)
        {
            if (a.type == animation::NORMAL)
            {
                a.current_time = a.duration_seconds;
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

        auto wa = which_array(a.element_id);
        if (wa == array_of::GROUPS)
        {
            group *owner = get_group(s, a.element_id);
            switch (a.p)
            {
                case animation::POSITION_X: owner->position.x = new_value;
                    break;
                case animation::POSITION_Y: owner->position.y = new_value;
                    break;
                case animation::ROTATION: owner->rotation = new_value;
                    break;
                default:
                    ASSERT_FAIL();
            }
        }
        if (wa == array_of::SHAPES)
        {
            shape *owner = get_shape(s, a.element_id);
            switch (a.p)
            {
                case animation::POSITION_X: owner->position.x = new_value;
                    break;
                case animation::POSITION_Y: owner->position.y = new_value;
                    break;
                case animation::ROTATION: owner->rotation = new_value;
                    break;
                case animation::WIDTH: owner->width = new_value;
                    break;
                case animation::HEIGHT: owner->height = new_value;
                    break;
                case animation::COLOR_R: owner->color.r = new_value;
                    break;
                case animation::COLOR_G: owner->color.g = new_value;
                    break;
                case animation::COLOR_B: owner->color.b = new_value;
                    break;
                case animation::COLOR_A: owner->color.a = new_value;
                    break;
                default:
                    ASSERT_FAIL();
            }
        }


        if (a.current_time >= a.duration_seconds)
        {
            s->animations.erase_not_sorted(i);
        }
        else
        {
            i += 1;
        }
    }
}

void update(system *s, input *inp)
{
    auto mouse_position = V3(inp->mouse.x, inp->mouse.y, 0);
    s->pressed = null_handle();

    uint32 order_index = 0;
    update_order_index_for_group(s, &s->root, order_index);

    update_animations(s, inp);

    hover_behaviour *hovered = NULL;

    for (usize i = 0; i < s->hoverables.size; i++)
    {
        hover_behaviour *hoverable = s->hoverables.data + i;

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
        if (s->active == null_handle())
        {
            // I found element under mouse, and no element is active! That's good, I can set myself as hot.
            if (s->hot != null_handle())
            {
                // There's something hot already, check if it's what I found.
                if (s->hot == hovered->owner->id)
                {
                    // The element I found under the mouse is exactly what I have hot. I will do nothing then.
                }
                else
                {
                    // This is new element! Remove hot from old one and make hot a new one!
                    make_cold(s, s->hot);
                    make_hot(s, hovered->owner);
                }
            }
            else
            {
                // There's nothing hot yet, let's make our element hot.
                make_hot(s, hovered->owner);
            }
        }
        else
        {
            // When this happens?
        }
    }
    else
    {
        if (s->hot != null_handle())
        {
            // I didn't find anything under the mouse, but I have a hot element? Make it cold again.
            make_cold(s, s->hot);
        }
    }

    if (get_press_count(inp->mouse[mouse_device::lmb]))
    {
        s->active = s->hot;
        if (s->active)
        {
            ASSERT(which_array(s->active) == array_of::GROUPS);
            group *active = get_group(s, s->active);
            if (active->clickable)
            {
                if (s->active == s->hot)
                {
                    active->clickable->on_press_internal(s, active);
                    active->clickable->on_press(s, active);
                }
            }
        }
    }

    if (get_release_count(inp->mouse[mouse_device::lmb]))
    {
        if (s->active)
        {
            ASSERT(which_array(s->active) == array_of::GROUPS);
            group *active = get_group(s, s->active);
            if (active->clickable)
            {
                if (s->active == s->hot)
                {
                    active->clickable->on_release(s, active);
                    s->pressed = s->active;
                }
                active->clickable->on_release_internal(s, active);
            }
            s->active = null_handle();
        }
    }

    // @note: This should be applied each frame after update phase, right?
    update_transforms(s);
}

void draw(execution_context *context, system *s)
{
    auto projection =
        math::translated(V3(-1, 1, 0),
        math::scaled(V3(2.0/context->letterbox_width, -2.0/context->letterbox_height, 1),
        math::matrix4::identity()));

    for (usize i = s->shapes.size - 1; i < s->shapes.size; i--)
    {
        shape *e = s->shapes.data + i;

        auto model =
            math::scaled(V3(0.5f * e->width, 0.5f * e->height, 1),
            math::to_matrix4(e->transform_to_root));

        render_command::command_draw_ui command_draw_ui;
        command_draw_ui.mesh_token = s->rectangle_mesh;
        command_draw_ui.shader_token = s->rectangle_shader;
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

void animate(system *s, group *e, animation::property property, animation::kind type, uint32 length_in_frames, float32 start_value, float32 final_value)
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


void animate_normal(system *s, group *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::NORMAL, length_in_frames, start_value, final_value);
}


void animate_normal_loop(system *s, group *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::NORMAL_LOOP, length_in_frames, start_value, final_value);
}


void animate_ping_pong(system *s, group *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::PING_PONG, length_in_frames, start_value, final_value);
}

void animate(system *s, shape *e, animation::property property, animation::kind type, uint32 length_in_frames, float32 start_value, float32 final_value)
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


void animate_normal(system *s, shape *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::NORMAL, length_in_frames, start_value, final_value);
}


void animate_normal_loop(system *s, shape *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::NORMAL_LOOP, length_in_frames, start_value, final_value);
}


void animate_ping_pong(system *s, shape *e, animation::property property, uint32 length_in_frames, float32 start_value, float32 final_value)
{
    animate(s, e, property, animation::PING_PONG, length_in_frames, start_value, final_value);
}


void delete_animation(system *s, uint32& index)
{
    if (index < s->animations.size)
        s->animations.erase_not_sorted(index);
    index = (uint32) -1;
}


bool32 button(system *s, uint64 id)
{
    bool32 result = false;
    handle h = get_handle_from_hash_table(s, id);
    if (which_array(h) == array_of::GROUPS)
    {
        result = (h == s->pressed);
    }
    return result;
}


} // namespace ui

#endif // UI_HPP

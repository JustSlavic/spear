#include "ui.hpp"

#include <memory_allocator.h>


namespace ui {


void callback_stub(system *, handle) {}


struct element
{
    handle parent;

    vector2 position;
    vector2 scale;
    float32 rotation;

    bool32 is_visible;

    // Cache
    transform tm;
    transform tm_to_root;
};

struct drawable
{
    handle owner;
    uint32 width;
    uint32 height;

    type_t type;
    union
    {
        vector4 color;
        resource_token texture_token;
    };
};

struct hoverable
{
    handle owner;
    math::rectangle2 area;

    hover_callbacks callbacks;
};

struct clickable
{
    handle owner;
    math::rectangle2 area;

    click_callbacks callbacks;
};

struct animation
{
    handle owner;
    uint32 type;

    string_id id;

    bool32 is_dormant;
    bool32 is_forward;

    float32 start_value;
    float32 final_value;

    // Assume begin time be 0s,
    // so duration is the same as "end time"
    float32 duration_seconds;
    float32 current_time;
};

struct attach
{
    handle h;
    attach *next;
};

struct system
{

    // Memory
    memory_allocator ui_allocator;
    string_id_storage *strid_storage;

    // Root
    element root;

    // Interactive stuff
    handle hot; // The element above which is the mouse
    handle active; // The element which player is interacting with

    // Primary elements
    static_array<element, 32> elements;
    // The array of linked lists of all the attaches for the particular element
    static_array<attach *, 32> attaches;
    // Secondary elements
    static_array<drawable, 32> drawables;
    static_array<hoverable, 32> hoverables;
    static_array<clickable, 32> clickables;
    static_array<animation, 32> animations;

    // Rendering stuff
    resource_token rectangle_mesh;
    resource_token rectangle_shader;
    resource_token rectangle_mesh_uv;
    resource_token rectangle_shader_uv;
};

system *initialize(memory_block ui_memory)
{
    memory_allocator arena = memory_allocator__create_arena_from_memory_block(ui_memory);

    system *s = ALLOCATE(arena, system);
    s->ui_allocator = arena;

    return s;
}

void push_attach_to_slot(system *s, attach *slot, handle child)
{
    while (slot->next)
    {
        slot = slot->next;
    }
    if (slot->h)
    {
        slot->next = ALLOCATE(s->ui_allocator, attach);
        slot->next->h = child;
    }
    else
    {
        slot->h = child;
    }
}

void attach_child(system *s, handle parent, handle child)
{
    if (parent.type == UI_ELEMENT)
    {
        if (s->attaches[parent.index] == NULL)
        {
            s->attaches[parent.index] = ALLOCATE(s->ui_allocator, attach);
            s->attaches[parent.index]->h = child;
        }
        else
        {
            auto *slot = s->attaches[parent.index];
            while (slot->next)
            {
                slot = slot->next;
            }
            slot->next = ALLOCATE(s->ui_allocator, attach);
            slot->next->h = child;
        }
    }
}

attach_iterator iterate_attaches(system *s, handle parent)
{
    attach_iterator result = { s->attaches[parent.index] };
    return result;
}

void set_string_id_storage(system *s, string_id_storage *storage)
{
    s->strid_storage = storage;
}

void set_resource_rectangle_mesh(system *s, resource_token mesh)
{
    s->rectangle_mesh = mesh;
}

void set_resource_rectangle_shader(system *s, resource_token shader)
{
    s->rectangle_shader = shader;
}

void set_resource_rectangle_mesh_uv(system *s, resource_token mesh)
{
    s->rectangle_mesh_uv = mesh;
}

void set_resource_rectangle_shader_uv(system *s, resource_token shader)
{
    s->rectangle_shader_uv = shader;
}

struct element_ref
{
    handle h;
    element *p;
};

element_ref push_element(system *s)
{
    ASSERT(s->elements.size() < s->elements.capacity());

    element_ref result;
    result.h.type = UI_ELEMENT;
    result.h.index = (uint32) s->elements.size();
    result.p = s->elements.push();
    result.p->is_visible = true;
    return result;
}

struct drawable_ref
{
    handle h;
    drawable *p;
};

drawable_ref push_shape(system *s)
{
    ASSERT(s->drawables.size() < s->drawables.capacity());

    drawable_ref result;
    result.h.type = UI_SHAPE;
    result.h.index = (uint32) s->drawables.size();
    result.p = s->drawables.push();
    return result;
}

drawable_ref push_image(system *s)
{
    ASSERT(s->drawables.size() < s->drawables.capacity());

    drawable_ref result;
    result.h.type = UI_IMAGE;
    result.h.index = (uint32) s->drawables.size();
    result.p = s->drawables.push();
    return result;
}

struct hoverable_ref
{
    handle h;
    hoverable *p;
};

hoverable_ref push_hoverable(system *s)
{
    ASSERT(s->hoverables.size() < s->hoverables.capacity());

    hoverable_ref result;
    result.h.type = UI_HOVERABLE;
    result.h.index = (uint32) s->hoverables.size();
    result.p = s->hoverables.push();
    return result;
}

struct clickable_ref
{
    handle h;
    clickable *p;
};

clickable_ref push_clickable(system *s)
{
    ASSERT(s->clickables.size() < s->clickables.capacity());

    clickable_ref result;
    result.h.type = UI_CLICKABLE;
    result.h.index = (uint32) s->clickables.size();
    result.p = s->clickables.push();
    return result;
}

struct animation_ref
{
    handle h;
    animation *p;
};

animation_ref push_animation(system *s)
{
    ASSERT(s->animations.size() < s->animations.capacity());

    animation_ref result;
    result.h.type = UI_ANIMATION;
    result.h.index = (uint32) s->animations.size();
    result.p = s->animations.push();
    return result;
}

handle make_group(system *s, handle parent)
{
    auto child = push_element(s);
    attach_child(s, parent, child.h);

    child.p->position = V2(0);
    child.p->scale    = V2(1);
    child.p->rotation = 0.f;
    child.p->parent   = parent;

    return child.h;
}

handle make_group(system *s)
{
    handle root_handle;
    root_handle.type = UI_ROOT;
    root_handle.index = 0;

    handle result = make_group(s, root_handle);
    return result;
}

handle make_shape(system *s, handle parent)
{
    auto child = push_element(s);
    auto graphics = push_shape(s);
    attach_child(s, parent, child.h);
    attach_child(s, child.h, graphics.h);

    child.p->position = V2(0);
    child.p->scale    = V2(1);
    child.p->rotation = 0.f;
    child.p->parent   = parent;

    graphics.p->owner  = child.h;
    graphics.p->width  = 100;
    graphics.p->height = 100;
    graphics.p->type   = UI_SHAPE;
    graphics.p->color  = V4(1);

    return child.h;
}

handle make_shape(system *s)
{
    handle root_handle;
    root_handle.type = UI_ROOT;
    root_handle.index = 0;

    handle result = make_shape(s, root_handle);
    return result;
}

handle make_image(system *s, handle parent)
{
    auto child = push_element(s);
    auto image = push_image(s);
    attach_child(s, parent, child.h);
    attach_child(s, child.h, image.h);

    child.p->position = V2(0);
    child.p->scale    = V2(1);
    child.p->rotation = 0.f;
    child.p->parent   = parent;

    image.p->owner  = child.h;
    image.p->width  = 100;
    image.p->height = 100;
    image.p->type   = UI_IMAGE;

    return child.h;
}

handle make_image(system *s)
{
    handle root_handle;
    root_handle.type = UI_ROOT;
    root_handle.index = 0;

    handle result = make_image(s, root_handle);
    return result;
}

hover_callbacks *make_hoverable(system *s, handle owner, int32 width, int32 height)
{
    auto child = push_hoverable(s);
    attach_child(s, owner, child.h);

    child.p->owner = owner;
    child.p->area = math::rectangle2::from_center_size(V2(0), (float32)width, (float32)height);
    child.p->callbacks.on_enter = callback_stub;
    child.p->callbacks.on_leave = callback_stub;
    child.p->callbacks.on_enter_internal = callback_stub;
    child.p->callbacks.on_leave_internal = callback_stub;

    return &(child.p->callbacks);
}

click_callbacks *make_clickable(system *s, handle owner, int32 width, int32 height)
{
    auto child = push_clickable(s);
    attach_child(s, owner, child.h);

    child.p->owner = owner;
    child.p->area = math::rectangle2::from_center_size(V2(0), (float32)width, (float32)height);
    child.p->callbacks.on_press = callback_stub;
    child.p->callbacks.on_release = callback_stub;
    child.p->callbacks.on_press_internal = callback_stub;
    child.p->callbacks.on_release_internal = callback_stub;

    return &(child.p->callbacks);
}

void update_animations(system *s, float32 dt)
{
    for (usize i = 0; i < s->animations.size(); i++)
    {
        auto *a = s->animations.data() + i;
        if (a->is_dormant) continue;

        float32 t = a->current_time + (a->is_forward ? 1.f : -1.f) * dt;

        anim_t type = (anim_t) (a->type & UI_ANIM_TYPE_MASK);
        anim_t prop = (anim_t) (a->type & UI_ANIM_PROPERTY_MASK);

        if (t > a->duration_seconds)
        {
            if (type == UI_ANIM_NORMAL)
            {
                t = a->duration_seconds;
                if (a->is_forward)
                {
                    a->is_dormant = true;
                }
            }
            if (type == UI_ANIM_LOOP)
            {
                t = (t - a->duration_seconds);
            }
            if (type == UI_ANIM_PPONG)
            {
                t = 2.f * a->duration_seconds - t;
                a->is_forward = false;
            }
        }
        if (t < 0.f)
        {
            if (type == UI_ANIM_NORMAL)
            {
                t = 0.f;
                if (!a->is_forward)
                {
                    a->is_dormant = true;
                }
            }
            if (type == UI_ANIM_LOOP)
            {
                t = (t + a->duration_seconds);
            }
            if (type == UI_ANIM_PPONG)
            {
                t = (-t);
                a->is_forward = true;
            }
        }

        a->current_time = t;
        float32 value = math::lerp(a->start_value, a->final_value, a->current_time / a->duration_seconds);

        switch (prop)
        {
            case UI_ANIM_POSITION_X:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    auto *p = s->elements.data() + a->owner.index;
                    p->position.x = value;
                }
            }
            break;

            case UI_ANIM_POSITION_Y:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    auto *p = s->elements.data() + a->owner.index;
                    p->position.y = value;
                }
            }
            break;

            case UI_ANIM_SCALE_X:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    auto *p = s->elements.data() + a->owner.index;
                    p->scale.x = value;
                }
            }
            break;

            case UI_ANIM_SCALE_Y:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    auto *p = s->elements.data() + a->owner.index;
                    p->scale.y = value;
                }
            }
            break;

            case UI_ANIM_ROTATION:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    auto *p = s->elements.data() + a->owner.index;
                    p->rotation = value;
                }
            }
            break;

            case UI_ANIM_WIDTH:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    for (auto h : iterate_attaches(s, a->owner))
                    {
                        if (h.type == UI_SHAPE)
                        {
                            auto *p = s->drawables.data() + h.index;
                            p->width = (uint32) value;
                        }
                    }
                }
            }
            break;

            case UI_ANIM_HEIGHT:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    for (auto h : iterate_attaches(s, a->owner))
                    {
                        if (h.type == UI_SHAPE)
                        {
                            auto *p = s->drawables.data() + h.index;
                            p->height = (uint32) value;
                        }
                    }
                }
            }
            break;

            case UI_ANIM_COLOR_R:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    for (auto h : iterate_attaches(s, a->owner))
                    {
                        if (h.type == UI_SHAPE)
                        {
                            auto *p = s->drawables.data() + h.index;
                            p->color.r = value;
                        }
                    }
                }
            }
            break;

            case UI_ANIM_COLOR_G:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    for (auto h : iterate_attaches(s, a->owner))
                    {
                        if (h.type == UI_SHAPE)
                        {
                            auto *p = s->drawables.data() + h.index;
                            p->color.g = value;
                        }
                    }
                }
            }
            break;

            case UI_ANIM_COLOR_B:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    for (auto h : iterate_attaches(s, a->owner))
                    {
                        if (h.type == UI_SHAPE)
                        {
                            auto *p = s->drawables.data() + h.index;
                            p->color.b = value;
                        }
                    }
                }
            }
            break;

            case UI_ANIM_COLOR_A:
            {
                if (a->owner.type == UI_ELEMENT)
                {
                    for (auto h : iterate_attaches(s, a->owner))
                    {
                        if (h.type == UI_SHAPE)
                        {
                            auto *p = s->drawables.data() + h.index;
                            p->color.a = value;
                        }
                    }
                }
            }
            break;


            case UI_ANIM_NORMAL:
            case UI_ANIM_LOOP:
            case UI_ANIM_PPONG:
            case UI_ANIM_PROPERTY_MASK:
            case UI_ANIM_TYPE_MASK:
            break;
        }
    }
}

void update_transforms(system *s)
{
    // @todo: do BFS here
    for (usize i = 0; i < s->elements.size(); i++)
    {
        auto *e = s->elements.data() + i;
        e->tm =
            transform__translate(V3(e->position, 0)) *
            transform__rotate_z(to_radians(e->rotation)) *
            transform__scale(V3(e->scale, 1));

        if (e->parent.type == UI_ROOT)
        {
            e->tm_to_root = e->tm;
        }
        else
        {
            auto *parent = s->elements.data() + e->parent.index;
            e->tm_to_root = parent->tm_to_root * e->tm;
        }
    }
}

void update(system *s, input_state *inp)
{
    auto mouse_position = V3(inp->mouse.x, inp->mouse.y, 0);

    update_animations(s, inp->dt);
    update_transforms(s);

    handle hovered = null_handle();
    for (usize i = 0; i < s->hoverables.size(); i++)
    {
        hoverable *h = s->hoverables.data() + i;

        ASSERT(h->owner.type == UI_ELEMENT);
        element *owner = s->elements.data() + h->owner.index;

        auto inverse_transform = inverse(owner->tm_to_root);
        auto mouse_position_local = transform_point(inverse_transform, mouse_position);

        if (math::is_inside(h->area, mouse_position_local.xy))
        {
            if (!hovered)
            {
                hovered.type = UI_ELEMENT;
                hovered.index = (uint32) h->owner.index;
            }
            // @todo: implement order index again ?
        }
    }

    if (hovered)
    {
        if (!s->active)
        {
            // I found element under mouse, and no element is active! That's good, I can set myself as hot.
            if (s->hot)
            {
                // There's something hot already, check if it's what I found.
                if (s->hot == hovered)
                {
                    // The element I found under the mouse is exactly what I have hot. I will do nothing then.
                }
                else
                {
                    for (auto child : iterate_attaches(s, s->hot))
                    {
                        if (child.type == UI_HOVERABLE)
                        {
                            auto *p = s->hoverables.data() + child.index;
                            p->callbacks.on_leave_internal(s, s->hot);
                            p->callbacks.on_leave(s, s->hot);
                            break;
                        }
                    }
                    s->hot = hovered;
                    for (auto child : iterate_attaches(s, s->hot))
                    {
                        if (child.type == UI_HOVERABLE)
                        {
                            auto *p = s->hoverables.data() + child.index;
                            p->callbacks.on_enter(s, s->hot);
                            p->callbacks.on_enter_internal(s, s->hot);
                        }
                    }
                }
            }
            else
            {
                // There's nothing hot yet, let's make our element hot.
                s->hot = hovered;
                for (auto child : iterate_attaches(s, s->hot))
                {
                    if (child.type == UI_HOVERABLE)
                    {
                        auto *p = s->hoverables.data() + child.index;
                        p->callbacks.on_enter(s, s->hot);
                        p->callbacks.on_enter_internal(s, s->hot);
                    }
                }
            }
        }
        else
        {
            // If there's already something active
            if (s->hot == hovered)
            {
                // What I found is already hot, do nothing
            }
            else if (hovered == s->active)
            {
                // What I found is the thing I have active, but it's not hot, make it hot again
                s->hot = hovered;
                for (auto child : iterate_attaches(s, s->hot))
                {
                    if (child.type == UI_HOVERABLE)
                    {
                        auto *p = s->hoverables.data() + child.index;
                        p->callbacks.on_enter(s, s->hot);
                        p->callbacks.on_enter_internal(s, s->hot);
                    }
                }
            }
        }
    }
    else
    {
        if (s->hot)
        {
            // I didn't find anything under the mouse, but I have a hot element? Make it cold again.
            for (auto child : iterate_attaches(s, s->hot))
            {
                if (child.type == UI_HOVERABLE)
                {
                    auto *p = s->hoverables.data() + child.index;
                    p->callbacks.on_leave_internal(s, s->hot);
                    p->callbacks.on_leave(s, s->hot);
                    break;
                }
            }
            s->hot = null_handle();
        }
    }

    if (get_press_count(inp->mouse[MOUSE_LEFT]))
    {
        s->active = s->hot;
        if (s->active)
        {
            for (auto child : iterate_attaches(s, s->active))
            {
                if (child.type == UI_CLICKABLE)
                {
                    if (s->active == s->hot)
                    {
                        auto *p = s->clickables.data() + child.index;
                        p->callbacks.on_press_internal(s, s->active);
                        p->callbacks.on_press(s, s->active);
                    }

                    break;
                }
            }
        }
    }

    if (get_release_count(inp->mouse[MOUSE_LEFT]))
    {
        if (s->active)
        {
            for (auto child : iterate_attaches(s, s->active))
            {
                if (child.type == UI_CLICKABLE)
                {
                    auto *p = s->clickables.data() + child.index;
                    if (s->active == s->hot)
                    {
                        p->callbacks.on_release(s, s->active);
                    }
                    p->callbacks.on_release_internal(s, s->active);
                }
            }
            s->active = null_handle();
        }
    }
}

void render(execution_context *context, system *s)
{
    auto projection =
        matrix4__translate(-1, 1, 0) *
        matrix4__scale(2.0f/context->letterbox_width, -2.0f/context->letterbox_height, 1);

    for (usize index = s->drawables.size() - 1; index < s->drawables.size(); index--)
    {
        auto *drawable = s->drawables.data() + index;
        auto *element = s->elements.data() + drawable->owner.index;

        if (element->is_visible == false) continue;

        if (drawable->type == UI_SHAPE)
        {
            auto model = transform__to_matrix4(element->tm_to_root) *
                matrix4__scale(0.5f*drawable->width, 0.5f*drawable->height, 1);

            render_command::command_draw_ui command_draw_ui;
            command_draw_ui.mesh_token = s->rectangle_mesh;
            command_draw_ui.shader_token = s->rectangle_shader;

            command_draw_ui.model = model; // @todo: remove transpose after I make all matrix4 be m * v instead of v * m as for now
            command_draw_ui.view = matrix4__identity();
            command_draw_ui.projection = projection;
            command_draw_ui.color = drawable->color;

            render_command cmd;
            cmd.type = render_command::command_type::draw_ui;
            cmd.draw_ui = command_draw_ui;

            push_render_command(context, cmd);
        }
        else if (drawable->type == UI_IMAGE)
        {
            auto scale_factor = 0.5f*(drawable->width > drawable->height ? drawable->width : drawable->height);
            auto model = transform__to_matrix4(element->tm_to_root) *
                matrix4__scale(scale_factor, scale_factor, 1);

            render_command::command_draw_ui_texture cmd;
            cmd.mesh_token = s->rectangle_mesh_uv;
            cmd.shader_token = s->rectangle_shader_uv;
            cmd.texture_token = drawable->texture_token;

            cmd.model = model;
            cmd.view = matrix4__identity();
            cmd.projection = projection;

            push_draw_ui_texture_command(context, cmd);
        }
    }
}

handle null_handle()
{
    handle result;
    result.type = UI_NONE;
    result.index = 0;

    return result;
}

handle make_handle(type_t t, uint32 index)
{
    handle result;
    result.type = t;
    result.index = index;

    return result;
}

handle::operator bool() const
{
    return (type != UI_NONE);
}

bool32 operator == (handle a, handle b)
{
    bool32 result = (a.type == UI_NONE && b.type == UI_NONE)
                 || (a.type == b.type && a.index == b.index);
    return result;
}

bool32 operator != (handle a, handle b)
{
    bool32 result = !(a == b);
    return result;
}

attach_iterator& attach_iterator::operator ++ ()
{
    p = p->next;;
    return *this;
}

attach_iterator attach_iterator::operator ++ (int)
{
    attach_iterator result = *this;
    p = p->next;
    return result;
}

bool attach_iterator::operator == (attach_iterator other)
{
    bool result = (p == other.p);
    return result;
}

bool attach_iterator::operator != (attach_iterator other)
{
    return !(*this == other);
}

handle attach_iterator::operator * () const
{
    return p->h;
}

handle attach_iterator::operator [] (int index) const
{
    auto q = p;
    while (index > 0)
    {
        q = q->next;
        index -= 1;
    }
    return q->h;
}

attach_iterator::operator bool()
{
    return p;
}

attach_iterator begin(attach_iterator it)
{
    return it;
}

attach_iterator end(attach_iterator it)
{
    attach_iterator result = {};
    return result;
}

bool get_visible(system *s, handle h)
{
    bool result = false;
    if (h.type == UI_ELEMENT)
    {
        element *e = s->elements.data() + h.index;
        result = e->is_visible;
    }
    return result;
}

void set_position(system *s, handle h, vector2 position)
{
    if (h.type == UI_ELEMENT)
    {
        element *e = s->elements.data() + h.index;
        e->position = position;
    }
}

void set_scale(system *s, handle h, vector2 scale)
{
    if (h.type == UI_ELEMENT)
    {
        element *e = s->elements.data() + h.index;
        e->scale = scale;
    }
}

void set_rotation(system *s, handle h, float32 rotation)
{
    if (h.type == UI_ELEMENT)
    {
        element *e = s->elements.data() + h.index;
        e->rotation = rotation;
    }
}

void set_color(system *s, handle h, vector4 color)
{
    for (auto a : iterate_attaches(s, h))
    {
        if (a.type == UI_SHAPE)
        {
            drawable *p = s->drawables.data() + a.index;
            p->color = color;
        }
    }
}

void set_texture(system *s, handle h, resource_token token)
{
    for (auto a : iterate_attaches(s, h))
    {
        if (a.type == UI_IMAGE)
        {
            drawable *p = s->drawables.data() + a.index;
            p->texture_token = token;
            auto *r = get_texture_resource(token);
            p->width = r->texture.width;
            p->height = r->texture.height;
        }
    }
}

void set_visible(system *s, handle h, bool is_visible)
{
    if (h.type == UI_ELEMENT)
    {
        element *e = s->elements.data() + h.index;
        e->is_visible = is_visible;
    }
}

void animate(system *s, handle h, string_id id, uint32 a, uint32 duration_frames, float32 start_value, float32 final_value)
{
    auto anim = push_animation(s);
    attach_child(s, h, anim.h);

    anim.p->owner = h;
    anim.p->type  = a;
    anim.p->id    = id;
    anim.p->is_dormant  = true;
    anim.p->is_forward  = true;
    anim.p->start_value = start_value;
    anim.p->final_value = final_value;

    anim.p->duration_seconds = (float32) duration_frames / 60.f;
    anim.p->current_time = 0.f;
}

void play_animation(system *s, char const *cstr)
{
    play_animation(s, make_string_id(s->strid_storage, cstr));
}

void play_animation(system *s, string_id id)
{
    for (usize i = 0; i < s->animations.size(); i++)
    {
        auto *a = s->animations.data() + i;
        if (a->id == id)
        {
            a->is_dormant = false;
            a->current_time = 0.f;

            break;
        }
    }
}

void stop_animation(system *s, char const *cstr)
{
    stop_animation(s, make_string_id(s->strid_storage, cstr));
}

void stop_animation(system *s, string_id id)
{
    for (usize i = 0; i < s->animations.size(); i++)
    {
        auto *a = s->animations.data() + i;
        if (a->id == id)
        {
            a->is_dormant = true;
            break;
        }
    }
}


} // namespace ui

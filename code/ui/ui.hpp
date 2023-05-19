#ifndef UI_HPP
#define UI_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <math/vector2.hpp>
#include <math/vector4.hpp>
#include <math/transform.hpp>
#include <array.hpp>
#include <input.hpp>


namespace ui {


struct element
{
    enum element_type
    {
        group,
        shape,
    };

    element_type  type;
    math::transform transform; // transform * vector_in_child => vector_in_parent
    math::vector3 position;  // in parent coordinates

    element *parent;
    // @todo: make it grow if needed? bucket_array?
    static_array<element *, 10> children;

    math::vector4 color; // shape's color
    math::transform transform_to_root; // Cached matrix to transform vectors from child space to root's
};

struct system
{
    memory::allocator ui_allocator;
    // @todo: expand it to grow in needed? separate pool allocators for each type of elements?
    static_array<element, 10> groups;
    static_array<element, 10> shapes;

    element root;
    element *hot; // The element is about to be interacted with (mouse over)
    element *active; // The element is being interacted with right now (mouse is down)

    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;
};

void create_root(system *sys)
{
    auto *root = &sys->root;
    memory::set(root, 0, sizeof(element));
    root->type = element::group;
    root->transform = math::transform::identity();
    root->transform_to_root = math::transform::identity();
}

element *create_child_group(system *sys, element *parent)
{
    element *result = NULL;
    if (parent->type == element::group)
    {
        result = sys->groups.push();
        memory::set(result, 0, sizeof(element));
        result->type = element::group;
        result->transform = math::transform::identity();

        result->parent = parent;
        parent->children.push(result);
    }
    return result;
}

element *create_child_shape(system *sys, element *parent)
{
    element *result = NULL;
    if (parent->type == element::group)
    {
        result = sys->shapes.push();
        memory::set(result, 0, sizeof(element));
        result->type = element::shape;
        result->transform = math::transform::identity();
        result->transform_to_root = math::transform::identity();

        result->parent = parent;
        parent->children.push(result);
    }
    return result;
}

void update_transform(element *e)
{
    // @todo: make rotations and scales too
    auto transform = math::transform::identity();
    translate(transform, e->position);

    e->transform = transform;
}

void update_transforms(system *sys)
{
    for (usize i = 0; i < sys->groups.size; i++)
    {
        element *e = sys->groups.data + i;
        update_transform(e);
    }
    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        update_transform(e);
    }
}

void update_transforms_to_root(system *sys)
{
    // @todo: make BFS here (where to allocate queue? scratchpad memory?)
    for (usize i = 0; i < sys->groups.size; i++)
    {
        element *e = sys->groups.data + i;
        e->transform_to_root = e->transform * e->parent->transform_to_root;
    }
    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        e->transform_to_root = e->transform * e->parent->transform_to_root;
    }
}

void update(system *sys, input *inp)
{
    auto mouse_position = V3(inp->mouse.x, inp->mouse.y, 0);
    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->groups.data + i;
        auto inverse_transform = inverse(e->transform_to_root);
        auto mouse_position_local = inverse_transform * mouse_position;
        // @todo: check if mouse position inside a rectangle
        // @todo: Decide which rectangle is drawn on top, so the top one should be "hot" now

        UNUSED(mouse_position_local);
    }
}

void draw(execution_context *context, system *sys)
{
    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        // Let's say all shapes are squares 100x100 for now
        auto tl = e->transform_to_root * V3(-50, -50, 0);
        auto tr = e->transform_to_root * V3( 50, -50, 0);
        auto bl = e->transform_to_root * V3(-50,  50, 0);
        auto br = e->transform_to_root * V3( 50,  50, 0);

        auto center = (tl + br) * 0.5f;
        auto half_width  = center.x - tl.x;
        auto half_height = center.y - tl.y;

        UNUSED(tl);
        UNUSED(tr);
        UNUSED(bl);
        UNUSED(br);

        render_command::command_draw_ui command_draw_ui;
        command_draw_ui.mesh_token = sys->rectangle_mesh;
        command_draw_ui.shader_token = sys->rectangle_shader;
        command_draw_ui.model =
            math::translated(V3(center.x, center.y, 0),
            math::scaled(V3(half_width, half_height, 1),
                math::matrix4::identity()));
        command_draw_ui.view = math::matrix4::identity();
        command_draw_ui.projection = math::scaled(V3(1.0/1280.0, 1.0/890.0, 1), math::matrix4::identity());
        command_draw_ui.color = V4(1, 0, 0, 1);
    }
}


} // namespace ui

#endif // UI_HPP

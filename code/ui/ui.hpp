#ifndef UI_HPP
#define UI_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <execution_context.hpp>
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

    math::vector3 position;  // position of origin (pivot) in parent coordinates
    math::vector3 scale;
    math::vector4 color;

    // @note: width and height are only for shapes
    float32 width;
    float32 height;

    element *parent;
    static_array<element *, 10> children; // @todo: make it grow if needed? bucket_array?

    math::transform transform; // transform * vector_in_child => vector_in_parent
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
        result->scale = V3(1);
        result->transform = math::transform::identity();
        result->transform_to_root = math::transform::identity();

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
        result->width = 100.f;
        result->height = 100.f;
        result->scale = V3(1);
        result->transform = math::transform::identity();
        result->transform_to_root = math::transform::identity();

        result->parent = parent;
        parent->children.push(result);
    }
    return result;
}

void update_transform(element *e)
{
    // @todo: make rotations
    auto transform =
        math::scaled(e->scale,
        math::translated(e->position,
        math::transform::identity()));
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
        e->transform_to_root = e->parent->transform_to_root * e->transform;
    }
    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        e->transform_to_root = e->parent->transform_to_root * e->transform;
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
    auto projection =
        math::translated(V3(-1, 1, 0),
        math::scaled(V3(2.0/context->letterbox_width, -2.0/context->letterbox_height, 1),
        math::matrix4::identity()));

    for (usize i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        auto m = math::scaled(V3(0.5f * e->width, 0.5f * e->height, 1), math::to_matrix4(e->transform_to_root));

        render_command::command_draw_ui command_draw_ui;
        command_draw_ui.mesh_token = sys->rectangle_mesh;
        command_draw_ui.shader_token = sys->rectangle_shader;
        command_draw_ui.model = math::transposed(m); // @todo: remove transpose after I make all matrix4 be m * v instead of v * m as for now
        command_draw_ui.view = math::matrix4::identity();
        command_draw_ui.projection = projection;
        command_draw_ui.color = e->color;

        render_command cmd;
        cmd.type = render_command::command_type::draw_ui;
        cmd.draw_ui = command_draw_ui;

        push_render_command(context, cmd);
    }
}


} // namespace ui

#endif // UI_HPP

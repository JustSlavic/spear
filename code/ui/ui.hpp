#ifndef UI_HPP
#define UI_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <math/vector2.hpp>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>
#include <array.hpp>

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
    for (int i = 0; i < sys->groups.size; i++)
    {
        element *e = sys->groups.data + i;
        update_transform(e);
    }
    for (int i = 0; i < sys->shapes.inputsize; i++)
    {
        element *e = sys->shapes.data + i;
        update_transform(e);
    }
}

void update_transforms_to_root(system *sys)
{
    // @todo: make BFS here (where to allocate queue? scratchpad memory?)
    for (int i = 0; i < sys->groups.size; i++)
    {
        element *e = sys->groups.data + i;
        e->transform_to_root = e->transform * e->parent->transform_to_root;
    }
    for (int i = 0; i < sys->shapes.size; i++)
    {
        element *e = sys->shapes.data + i;
        e->transform_to_root = e->transform * e->parent->transform_to_root;
    }
}


} // namespace ui

#endif // UI_HPP

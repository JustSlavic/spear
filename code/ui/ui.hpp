#ifndef UI_HPP
#define UI_HPP

#include <base.hpp>
#include <memory/allocator.hpp>
#include <math/matrix2.hpp>
#include <array.hpp>

namespace ui {


struct element
{
    enum kind
    {
        group,
        shape,
    };

    kind type;
    math::matrix2 transform; // parent -> child
    math::vector2 position; // in parent coordinates

    element *parent;
    static_array<element *, 10> children;
};


struct system
{
    memory::allocator ui_allocator;
    static_array<element, 10> groups;
    static_array<element, 10> shapes;

    element *root;
};

element *create_root(system *sys)
{
    element *result = NULL;
    if (sys->root)
    {
        result = sys->root;
    }
    else
    {
        result = sys->groups.push();
        memory::set(result, 0, sizeof(element));
        result->type = element::group;
        result->transform = math::matrix2::identity();
    }
    return result;
}

element *create_child_group(system *sys, element *parent)
{
    element *result = NULL;
    if (parent->type == element::group)
    {
        result = sys->groups.push();
        memory::set(result, 0, sizeof(element));
        result->type = element::group;
        result->transform = math::matrix2::identity();

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
        result->transform = math::matrix2::identity();

        parent->children.push(result);
    }
    return result;
}


} // namespace ui

#endif // UI_HPP

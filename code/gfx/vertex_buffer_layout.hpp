#ifndef GFX_VERTEX_BUFFER_LAYOUT_HPP
#define GFX_VERTEX_BUFFER_LAYOUT_HPP

#include <base.hpp>


namespace gfx {


// @note: for now always floats
struct vertex_buffer_layout_element
{
    uint32 count;
};

struct vertex_buffer_layout
{
    vertex_buffer_layout_element elements[8];
    uint32 element_count;
};

INLINE void push_layout_element(vertex_buffer_layout *vbl, uint32 count)
{
    ASSERT(vbl->element_count < ARRAY_COUNT(vbl->elements));

    vbl->elements[vbl->element_count++].count = count;
}


} // namespace gfx

#endif // GFX_VERTEX_BUFFER_LAYOUT_HPP

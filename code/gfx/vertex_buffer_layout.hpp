#ifndef GFX_VERTEX_BUFFER_LAYOUT_HPP
#define GFX_VERTEX_BUFFER_LAYOUT_HPP

#include <base.h>


namespace gfx {


// @note: for now always floats
struct vertex_buffer_layout_element
{
    uint32 count;
    // @todo: other types of elements other than floats
};

struct vertex_buffer_layout
{
    vertex_buffer_layout_element elements[8];
    uint32 total_count;
    uint32 count;

    static vertex_buffer_layout make()
    {
        vertex_buffer_layout result = {};
        return result;
    }

    template <typename T>
    void push(uint32 count)
    {
        ASSERT(count < ARRAY_COUNT(elements));
        elements[count++].count = count;
    }
};



} // namespace gfx

#endif // GFX_VERTEX_BUFFER_LAYOUT_HPP

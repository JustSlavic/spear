#ifndef GFX_VIEWPORT_HPP
#define GFX_VIEWPORT_HPP

#include <base.hpp>


namespace gfx {

struct viewport
{
    uint32 offset_x;
    uint32 offset_y;
    uint32 width;
    uint32 height;
};

viewport make_viewport(uint32 width, uint32 height, float32 desired_aspect_ratio)
{
    viewport result;

    float32 aspect_ratio = (float32) width / (float32) height;
    if (aspect_ratio < desired_aspect_ratio)
    {
        // Black strips on top and bottom of the screen
        result.width    = width;
        result.height   = (uint32) (result.width / desired_aspect_ratio);
        result.offset_x = 0;
        result.offset_y = (height - result.height) / 2;
    }
    else if (aspect_ratio > desired_aspect_ratio)
    {
        // Black strips on left and right of the screen
        result.height   = height;
        result.width    = (uint32) (result.height * desired_aspect_ratio);
        result.offset_x = (width - result.width) / 2;
        result.offset_y = 0;
    }
    else
    {
        // No black strips
        result.width    = width;
        result.height   = height;
        result.offset_x = 0;
        result.offset_y = 0;
    }

    return result;
}

} // namespace gfx


#endif // GFX_VIEWPORT_HPP

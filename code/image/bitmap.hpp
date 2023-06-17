#ifndef IMAGE_BITMAP_HPP
#define IMAGE_BITMAP_HPP

#include <base.hpp>


enum color_order : uint32
{
    IMAGE_RGB,
    IMAGE_BGR,
    IMAGE_ARGB,
    IMAGE_ABGR,
    IMAGE_RGBA,
    IMAGE_BGRA,
};


namespace image {


struct bitmap
{
    uint8 *pixels;
    uint32 size;
    uint32 width;
    uint32 height;
    uint32 bytes_per_pixel;
    color_order  color_order;
    bool32 top_down;
};


} // namespace image


#endif // IMAGE_BITMAP_HPP

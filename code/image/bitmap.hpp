#ifndef IMAGE_BITMAP_HPP
#define IMAGE_BITMAP_HPP

#include <base.h>


enum color_t : uint32
{
    IMAGE_INVALID,
    // W - white (when grayscale)
    // R - Red
    // G - Green
    // B - Blue
    // A - Alpha
    IMAGE_W,
    IMAGE_WA,
    IMAGE_RGB,
    IMAGE_BGR,
    IMAGE_ARGB,
    IMAGE_ABGR,
    IMAGE_RGBA,
    IMAGE_BGRA,

    IMAGE_GRAYSCALE = IMAGE_W,
    IMAGE_GRAYSCALE_ALPHA = IMAGE_WA,
};


struct bitmap
{
    uint8 *pixels;
    uint32 size;
    uint32 width;
    uint32 height;
    uint32 bits_per_pixel;
    color_t color_type;
    bool32 top_down;
};


#endif // IMAGE_BITMAP_HPP

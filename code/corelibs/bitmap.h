#ifndef SPEAR_CORELIBS_BITMAP_H
#define SPEAR_CORELIBS_BITMAP_H

#include <corelibs/base.h>


enum
{
    Bitmap_W, // Grayscale
    Bitmap_WA, // Grayscale with transparency
    Bitmap_RGB,
    Bitmap_BGR,
    Bitmap_ARGB,
    Bitmap_ABGR,
    Bitmap_RGBA,
    Bitmap_BGRA,
};

typedef struct
{
    void *data;
    usize size;

    uint32 width;
    uint32 height;
    uint32 bits_per_pixel;
    uint32 color_mode;
} bitmap;


#endif // SPEAR_CORELIBS_BITMAP_H

#ifndef FRUSTRUM_HPP
#define FRUSTRUM_HPP

#include <base.h>


struct frustrum
{
    float32 near_clip_distance;
    float32 far_clip_distance;

    float32 near_clip_width;
    float32 near_clip_height;
};


#endif // FRUSTRUM_HPP

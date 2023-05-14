#ifndef TIME_HPP
#define TIME_HPP

#include <base.hpp>


struct timepoint
{
    uint64 counts;
};

struct duration
{
    uint64 counts;
};

float32 get_seconds(duration d)
{
    float32 result = (float32) d.counts / 1000000.0f;
    return result;
}

INLINE duration operator - (timepoint t1, timepoint t2)
{
    ASSERT(t2.counts < t1.counts);
    duration result = { t1.counts - t2.counts };
    return result;
}

INLINE timepoint operator + (timepoint t, duration d)
{
    timepoint result = { t.counts + d.counts };
    return result;
}

INLINE duration operator + (duration d1, duration d2)
{
    duration result = { d1.counts + d2.counts };
    return result;
}

INLINE duration operator - (duration d1, duration d2)
{
    duration result = { d1.counts - d2.counts };
    return result;
}

INLINE bool32 operator < (duration d1, duration d2)
{
    bool32 result = (d1.counts < d2.counts);
    return result;
}


#endif // TIME_HPP

#ifndef OS_TIME_HPP
#define OS_TIME_HPP

#include <base.hpp>

namespace os {


struct timepoint
{
    uint64 us_from_epoch;
};

struct duration
{
    uint64 us;
};

float32 get_seconds(duration d)
{
    float32 result = (float32) d.us / 1000000.0f;
    return result;
}

INLINE duration operator - (timepoint t1, timepoint t2)
{
    ASSERT(t2.us_from_epoch < t1.us_from_epoch);
    duration result = { t1.us_from_epoch - t2.us_from_epoch };
    return result;
}

INLINE timepoint operator + (timepoint t, duration d)
{
    timepoint result = { t.us_from_epoch + d.us };
    return result;
}

INLINE duration operator + (duration d1, duration d2)
{
    duration result = { d1.us + d2.us };
    return result;
}

INLINE duration operator - (duration d1, duration d2)
{
    duration result = { d1.us - d2.us };
    return result;
}

INLINE bool32 operator < (duration d1, duration d2)
{
    bool32 result = (d1.us < d2.us);
    return result;
}


} // namespace os

#endif // OS_TIME_HPP

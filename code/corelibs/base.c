#include "base.h"


bool is_near_zero_eps(float x, float eps)
{
    bool result = (-eps < x && x < eps);
    return result;
}

bool is_near_zero(float x)
{
    bool result = is_near_zero_eps(x, EPSILON);
    return result;
}

float square(float x)
{
    float result = x * x;
    return result;
}

float degrees_to_radians(float degrees)
{
    float result = degrees * PI / 180.0f;
    return result;
}

float radians_to_degrees(float radians)
{
    float result = radians * 180.0f / PI;
    return result;
}

float lerp(float a, float b, float t)
{
    float result = (1.f - t) * a + t * b;
    return result;
}

float clamp(float x, float a, float b)
{
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

float cvt(float x, float a, float b, float c, float d)
{
    float y = (clamp(x, a, b) - a) * (d - c) / (b - a) + c;
    return y;
}

int get_alignment(void *pointer)
{
    uint64 ptr = (uint64) pointer;
    if ((ptr & 0x7) == 0) return 8;
    if ((ptr & 0x3) == 0) return 4;
    if ((ptr & 0x1) == 0) return 2;
    return 1;
}

uint64 get_padding(void *pointer, uint64 alignment)
{
    uint64 result = (alignment - ((uint64) pointer & (alignment - 1))) & (alignment - 1);
    return result;
}

void *align_pointer(void *pointer, uint64 alignment)
{
    void *result = (byte *) pointer + get_padding(pointer, alignment);
    return result;
}

usize cstring_size_no0(char const *data)
{
    usize result = 0;
    if (data) while (*data++) result += 1;
    return result;
}

usize cstring_size_with0(char const *data)
{
    usize result = cstring_size_no0(data) + 1;
    return result;
}

memory_view memory_view_create(void *data, usize size)
{
    memory_view result = { .data = data, .size = size };
    return result;
}

bool32 memory_view_is_valid(memory_view view)
{
    bool32 result = view.data && view.size > 0;
    return result;
}


timepoint timepoint_create_seconds(uint64 s)
{
    timepoint result = { .microseconds = s * 1000000 };
    return result;
}

timepoint timepoint_create_milliseconds(uint64 ms)
{
    timepoint result = { .microseconds = ms * 1000 };
    return result;
}

timepoint timepoint_create_microseconds(uint64 us)
{
    timepoint result = { .microseconds = us };
    return result;
}

duration duration_create_seconds(uint64 s)
{
    duration result = { .microseconds = s * 1000000 };
    return result;
}

duration duration_create_milliseconds(uint64 ms)
{
    duration result = { .microseconds = ms * 1000 };
    return result;
}

duration duration_create_microseconds(uint64 us)
{
    duration result = { .microseconds = us };
    return result;
}

float64 timepoint_get_seconds(timepoint t)
{
    float64 result = t.microseconds / 1000000.0;
    return result;
}

float64 timepoint_get_milliseconds(timepoint t)
{
    float64 result = t.microseconds / 1000.0;
    return result;
}

float64 timepoint_get_microseconds(timepoint t)
{
    float64 result = (float64) t.microseconds;
    return result;
}

float64 duration_get_seconds(duration t)
{
    float64 result = t.microseconds / 1000000.0;
    return result;
}

float64 duration_get_milliseconds(duration t)
{
    float64 result = t.microseconds / 1000.0;
    return result;
}

float64 duration_get_microseconds(duration t)
{
    float64 result = (float64) t.microseconds;
    return result;
}

duration get_duration_between_timepoints(timepoint t1, timepoint t2)
{
    duration result = { .microseconds = t2.microseconds - t1.microseconds };
    return result;
}

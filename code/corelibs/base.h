#ifndef _SPEAR_BASE_H
#define _SPEAR_BASE_H

#if defined(_MSC_VER)
    #define COMPILER_MSVC 1
#elif defined(__GNUC__) && !defined(__clang__)
    #define COMPILER_GNU 1
#elif defined(__clang__)
    #define COMPILER_CLANG 1
#elif defined(__MINGW32__)
    #define COMPILER_MINGW 1
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS 1
#elif defined(__linux__) || defined(__linux) || defined(linux)
    #define OS_LINUX 1
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC 1
#elif defined(__FreeBSD__)
    #define OS_FREEBSD 1
#elif defined(__unix__) || defined(__unix) || defined(unix)
    // Nothing yet
#endif

#if COMPILER_MSVC
    typedef          __int8  int8;
    typedef          __int16 int16;
    typedef          __int32 int32;
    typedef          __int64 int64;
    typedef unsigned __int8  uint8;
    typedef unsigned __int16 uint16;
    typedef unsigned __int32 uint32;
    typedef unsigned __int64 uint64;
    typedef float            float32;
    typedef double           float64;

    #define FORCE_INLINE __forceinline
    #define DLL_EXPORT __declspec(dllexport)

    #if DEBUG
    #define DEBUG_BREAK __debugbreak
    #else
    #define DEBUG_BREAK
    #endif // DEBUG
#endif

#if COMPILER_CLANG
    typedef   signed char        int8;
    typedef   signed short       int16;
    typedef   signed int         int32;
    typedef   signed long long   int64;
    typedef unsigned char        uint8;
    typedef unsigned short       uint16;
    typedef unsigned int         uint32;
    typedef unsigned long long   uint64;
    typedef float                float32;
    typedef double               float64;

    #define FORCE_INLINE         __attribute__((always_inline)) inline
    #define DLL_EXPORT           extern

    #if DEBUG
    #define DEBUG_BREAK __builtin_trap
    #else
    #define DEBUG_BREAK
    #endif // DEBUG

    #define INFINITY (1.f / 0.f)
#endif

typedef uint32               uint;
typedef  int64               isize;
typedef uint64               usize;
typedef uint8                byte;
typedef uint8                bool;
typedef uint32               bool32;
typedef  int64               intptr;
typedef uint64               uintptr;
typedef int16                sound_sample_t;
#define true                 1
#define false                0
#define NULL                 (void *) 0

#if OS_MAC || OS_WINDOWS
    typedef int64 timestamp_t;
#endif

#define static_assert(expression, message) _Static_assert((expression), (message))
#define alignof(T) _Alignof(T)

#define REPORT_ERROR(...) do { fprintf(stderr, "[%s:%d] Error: ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr); } while(false)

#if DEBUG
#define ASSERT(COND)             do {if (!(COND)) { DEBUG_BREAK(); }} while(false)
#define ASSERT_MSG(COND, ...)    do {if (!(COND)) { REPORT_ERROR(__VA_ARGS__); DEBUG_BREAK(); }} while(false)
#define ASSERT_IF(COND)          if (!(COND)) { DEBUG_BREAK(); } else
#define ASSERT_MSG_IF(COND, ...) if (!(COND)) { REPORT_ERROR(__VA_ARGS__); DEBUG_BREAK(); } else
#else
#define ASSERT(COND)             (void)(0)
#define ASSERT_MSG(COND, ...)    (void)(0)
#define ASSERT_IF(COND)          if (COND)
#define ASSERT_MSG_IF(COND, ...) if (COND)
#endif // DEBUG

#define ARRAY_COUNT(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define GLSL(...) #__VA_ARGS__
#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)
#define UNUSED(X) (void) (X)
#define KILOBYTES(N) (1024ull * (N))
#define MEGABYTES(N) (1024ull * KILOBYTES(N))
#define GIGABYTES(N) (1024ull * MEGABYTES(N))
#define TERABYTES(N) (1024ull * GIGABYTES(N))
#define TOGGLE(X) (X) = !(X)

#define PRINT_BOOL(X) ((X) ? "true" : "false")

#define EPSILON  (1e-6)
#define PI       (3.1415926535897932384626433832795028841971693993751058209749445923078164062)
#define HALF_PI  (1.5707963267948966192313216916397514420985846996875529104874722961539082031)
#define TWO_PI   (6.2831853071795864769252867665590057683943387987502116419498891846156328125)

typedef struct
{
    char const *function;
    char const *filename;
    uint line;
} code_location;

#define CL_HERE ((code_location){ .function=__FUNCTION__, .filename=__FILE__, .line=__LINE__ })

float degrees_to_radians(float degrees);
float radians_to_degrees(float radians);

bool is_near_zero(float x);
bool is_near_zero_eps(float x, float eps);

float square(float x);
float lerp(float a, float b, float t);
float clamp(float x, float a, float b);
float cvt(float x, float a, float b, float c, float d);

int get_alignment(void *pointer);
uint64 get_padding(void *pointer, uint64 alignment);
void *align_pointer(void *pointer, uint64 alignment);

usize cstring_size_no0(char const *cstr);
usize cstring_size_with0(char const *cstr);

typedef struct
{
    void *data;
    usize size;
} memory_view;

memory_view memory_view_create(void *data, usize size);
bool32 memory_view_is_valid(memory_view view);

typedef struct
{
    uint64 microseconds;
} timepoint;

typedef struct
{
    uint64 microseconds;
} duration;

timepoint timepoint_create_seconds(uint64 s);
timepoint timepoint_create_milliseconds(uint64 s);
timepoint timepoint_create_microseconds(uint64 s);

duration duration_create_seconds(uint64 s);
duration duration_create_milliseconds(uint64 s);
duration duration_create_microseconds(uint64 s);

float64 timepoint_get_seconds(timepoint t);
float64 timepoint_get_milliseconds(timepoint t);
float64 timepoint_get_microseconds(timepoint t);

float64 duration_get_seconds(duration t);
float64 duration_get_milliseconds(duration t);
float64 duration_get_microseconds(duration t);

duration get_duration_between_timepoints(timepoint t1, timepoint t2);


#endif // _SPEAR_BASE_H

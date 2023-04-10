#ifndef BASE_HPP
#define BASE_HPP


#ifdef _MSC_VER
#define COMPILER_MSVC 1
#elif __GNUC__
#define COMPILER_GNU 1
#elif __clang__
#define COMPILER_CLANG 1
#elif __MINGW32__
#define COMPILER_MINGW 1
#endif


#ifdef COMPILER_MSVC

#define not !
#define and &&
#define or ||

#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN    4321

typedef __int8  int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef float  float32;
typedef double float64;

extern "C" uint64 __rdtsc(void);
#pragma intrinsic(__rdtsc)

#define FORCE_INLINE __forceinline
#define DLL_EXPORT __declspec(dllexport)

#define RELEASE_COM(PTR) do { if ((PTR)) { (PTR)->Release(); } (PTR) = NULL; } while(0)

#if DEBUG
#define DEBUG_BREAK __debugbreak
#define DEBUG_CYCLE_COUNT __rdtsc
#else
#define DEBUG_BREAK
#define DEBUG_CYCLE_COUNT
#endif

#endif // COMPILER_MSVC

#if COMPILER_GNU

typedef signed char        int8;
typedef signed short       int16;
typedef signed int         int32;
typedef signed long long   int64;

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef float  float32;
typedef double float64;

#define FORCE_INLINE
#define DLL_EXPORT

#define NULL ((void *) 0)

#if DEBUG
#define DEBUG_BREAK __builtin_trap
#define DEBUG_CYCLE_COUNT(...) 0 // @todo
#else
#define DEBUG_BREAK
#define DEBUG_CYCLE_COUNT
#endif

#endif // COMPILER_GNU

typedef uint8  byte;
typedef  int32 bool32;
typedef uint64 usize;
typedef  int64 isize;
typedef uint64 uintptr;
typedef  int64 intptr;
typedef  int64 ptrdiff;


// Sound sample is 16-bit number
// Sound frame is 2 samples: [left, right]
// Sound period is a number of frames
// Sound buffer is a bunch of periods?
typedef int16 sound_sample_t;

#define EPSILON    1e-6f

#define INT8_MIN   0x80
#define INT16_MIN  0x8000
#define INT32_MIN  0x80000000
#define INT64_MIN  0x8000000000000000

#define INT8_MAX   0x7F
#define INT16_MAX  0x7FFF
#define INT32_MAX  0x7FFFFFFF
#define INT64_MAX  0x7FFFFFFFFFFFFFFF

#define UINT8_MIN  0
#define UINT16_MIN 0
#define UINT32_MIN 0
#define UINT64_MIN 0

#define UINT8_MAX  0xFF
#define UINT16_MAX 0xFFFF
#define UINT32_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFF


#define CONSTANT   const
#define STATIC     static
#define INTERNAL   static
#define PERSIST    static
#define GLOBAL     static
#define INLINE     inline


#define ARRAY_COUNT(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))


#define KILOBYTES(VALUE) (((usize)(VALUE))*1024)
#define MEGABYTES(VALUE) (KILOBYTES((usize)(VALUE))*1024)
#define GIGABYTES(VALUE) (MEGABYTES((usize)(VALUE))*1024)
#define TERABYTES(VALUE) (GIGABYTES((usize)(VALUE))*1024)


#ifdef DEBUG
#define ASSERT(COND)          if (COND) {} else { DEBUG_BREAK(); } (void)(0)
#define ASSERT_MSG(COND, ...) if (COND) {} else { DEBUG_BREAK(); } (void)(0)
#else
#define ASSERT(COND)          (void)(0)
#define ASSERT_MSG(COND, ...) (void)(0)
#endif // DEBUG

#define ASSERT_FAIL(...) ASSERT_MSG(false, __VA_ARGS__)


#define STRINGIFY_(X)    #X
#define STRINGIFY(X)     STRINGIFY_(X)

#define UNUSED(X) (void) (X)

#define MACRO_EXPAND(X) X

#define MACRO_OVERLOAD_1(_1, NAME, ...) NAME
#define MACRO_OVERLOAD_2(_1, _2, NAME, ...) NAME
#define MACRO_OVERLOAD_3(_1, _2, _3, NAME, ...) NAME
#define MACRO_OVERLOAD_4(_1, _2, _3, _4, NAME, ...) NAME

#define loop while (true)

#define SWAP(X, Y) { decltype(X) tmp__ = X; Y = X; X = tmp__; } while(0)
template <typename T>
void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}


#endif // BASE_HPP

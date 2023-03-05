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

#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN    4321

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef float  float32;
typedef double float64;

#define DEBUG_BREAK __debugbreak
#define FORCE_INLINE __forceinline

#endif // COMPILER_MSVC


typedef  int32_t bool32_t;
typedef uint64_t size_t;
typedef  int64_t isize_t;
typedef uint64_t uintptr_t;
typedef  int64_t intptr_t;
typedef  int64_t ptrdiff_t;


// Sound sample is 16-bit number
// Sound frame is 2 samples: [left, right]
// Sound period is a number of frames
// Sound buffer is a bunch of periods?
typedef int16_t sound_sample_t;


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
#define ASSERT(COND)          if (COND) {} else { DEBUG_BREAK(); } void(0)
#define ASSERT_MSG(COND, ...) if (COND) {} else { DEBUG_BREAK(); } void(0)
#else
#define ASSERT(COND)          void(0)
#define ASSERT_MSG(COND, ...) void(0)
#endif // DEBUG

#define ASSERT_FAIL(...) ASSERT_MSG(false, __VA_ARGS__)


#define STRINGIFY_(X)    #X
#define STRINGIFY(X)     STRINGIFY_(X)



#endif // BASE_HPP

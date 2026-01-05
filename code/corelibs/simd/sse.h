#ifndef SPEAR_CORELIBS_SIMD_SSE_H_
#define SPEAR_CORELIBS_SIMD_SSE_H_

#if OS_WINDOWS
#include <xmmintrin.h>
#elif OS_LINUX
#include <immintrin.h>
#endif

typedef __m128 float4;
typedef __m128d double2;


#endif // SPEAR_CORELIBS_SIMD_SSE_H_

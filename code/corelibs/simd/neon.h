#ifndef SPEAR_CORELIBS_SIMD_NEON_H_
#define SPEAR_CORELIBS_SIMD_NEON_H_

#include <corelibs/base.h>
#include <arm_neon.h>


typedef float32x4_t float32x4;
typedef float64x2_t float64x2;


float32x4 v_make_ps(float32 x, float32 y, float32 z, float32 w) { return (float32x4){ x, y, z, w }; }
float32x4 v_splat_ps(float32 x) { return vdupq_n_f32(x); }

float32x4 v_set_x_ps(float32x4 v, float32 x) { return vsetq_lane_f32(x, v, 0); }
float32x4 v_set_y_ps(float32x4 v, float32 x) { return vsetq_lane_f32(x, v, 1); }
float32x4 v_set_z_ps(float32x4 v, float32 x) { return vsetq_lane_f32(x, v, 2); }
float32x4 v_set_w_ps(float32x4 v, float32 x) { return vsetq_lane_f32(x, v, 3); }

float32 v_get_x_ps(float32x4 v) { return vgetq_lane_f32(v, 0); }
float32 v_get_y_ps(float32x4 v) { return vgetq_lane_f32(v, 1); }
float32 v_get_z_ps(float32x4 v) { return vgetq_lane_f32(v, 2); }
float32 v_get_w_ps(float32x4 v) { return vgetq_lane_f32(v, 3); }

float32x4 v_add_ps(float32x4 a, float32x4 b) { return vaddq_f32(a, b); }
float32x4 v_sub_ps(float32x4 a, float32x4 b) { return vsubq_f32(a, b); }
float32x4 v_mul_ps(float32x4 a, float32x4 b) { return vmulq_f32(a, b); }
float32x4 v_div_ps(float32x4 a, float32x4 b) { return vdivq_f32(a, b); }

// a * b + c
float32x4 v_fma_ps(float32x4 a, float32x4 b, float32x4 c) { return vfmaq_f32(c, a, b); }

float32x4 v_dot_ps(float32x4 a, float32x4 b)
{
    float32x4 c = v_mul_ps(a, b);
    float32x4 d = vpaddq_f32(c, c);
    return vpaddq_f32(d, d);
}


#endif // SPEAR_CORELIBS_SIMD_NEON_H_

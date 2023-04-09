#ifndef GFX_DX_HPP
#define GFX_DX_HPP


#include <base.hpp>


namespace gfx {

namespace dx {


INLINE math::matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
{
    math::matrix4 Result = {};

    Result._11 = 2.0f * n / w;
    Result._22 = 2.0f * n / h;
    Result._33 = -f / (f - n);
    Result._34 = -1.0f;
    Result._43 = -f*n / (f - n);

    return Result;
}

INLINE math::matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    //     w/2
    //   +-----+
    //   |    /
    //   |   /
    // n |  /
    //   | / angle = fov/2
    //   |/  tg(fov / 2) = (w/2) / n
    //   +   => 2n / w = 1 / tg(fov / 2)

    float32 tf2 = (1.0f / tanf(0.5f * fov));

    math::matrix4 result = {};

    result._11 = tf2;
    result._22 = tf2 * aspect_ratio;
    result._33 = -f / (f - n);
    result._34 = -1.0f;
    result._43 = -f*n / (f - n);

    return result;
}

INLINE math::matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
{
    math::matrix4 result = {};

    result._11 = 2.0f / w;
    result._22 = 2.0f / h;
    result._33 = -1.0f / (f - n);
    result._43 = -n / (f - n);
    result._44 = 1.0f;

    return result;
}

INLINE math::matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f)
{
    math::matrix4 result;

    result._11 = 1.0f;
    result._22 = 1.0f * aspect_ratio;
    result._33 = -1.0f / (f - n);
    result._43 = -n / (f - n);
    result._44 = 1.0f;

    return result;
}


} // namespace dx

} // namespace gfx

#endif // GFX_DX_HPP

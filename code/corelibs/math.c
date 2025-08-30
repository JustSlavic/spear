#include "math.h"
#include <math.h>


vector2 vector2_create(float x, float y)
{
    vector2 result = {};
    result.x = x;
    result.y = y;
    return result;
}

vector2 vector2_negate(vector2 v)
{
    vector2 result = {};
    result.x = -v.x;
    result.y = -v.y;
    return result;
}

vector2 vector2_scale(float32 a, vector2 v)
{
    vector2 result = {};
    result.x = a * v.x;
    result.y = a * v.y;
    return result;
}

vector2 vector2_add(vector2 v, vector2 w)
{
    vector2 result = {};
    result.x = v.x + w.x;
    result.y = v.y + w.y;
    return result;
}

vector2 vector2_sub(vector2 v, vector2 w)
{
    vector2 result = {};
    result.x = v.x - w.x;
    result.y = v.y - w.y;
    return result;
}

float32 vector2_dot(vector2 a, vector2 b)
{
    float32 result = a.x * b.x + a.y * b.y;
    return result;
}

float vector2_norm_squared(vector2 v)
{
    float result = vector2_dot(v, v);
    return result;
}

float vector2_norm(vector2 v)
{
    float result = sqrtf(vector2_norm_squared(v));
    return result;
}

vector2 vector2_normalize(vector2 v)
{
    float norm = vector2_norm(v);
    if (norm > 0)
    {
        v.x = v.x / norm;
        v.y = v.y / norm;
    }
    return v;
}

// ============ vector3 ============ //

vector3 vector3_create(float x, float y, float z)
{
    vector3 result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

vector3 vector3_negate(vector3 v)
{
    vector3 result = {};
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    return result;
}

vector3 vector3_scale(float32 a, vector3 v)
{
    vector3 result = {};
    result.x = a * v.x;
    result.y = a * v.y;
    result.z = a * v.z;
    return result;
}

vector3 vector3_add(vector3 a, vector3 b)
{
    vector3 result = {};
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vector3 vector3_sub(vector3 a, vector3 b)
{
    vector3 result = {};
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

float32 vector3_dot(vector3 a, vector3 b)
{
    float32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

vector3 vector3_cross(vector3 a, vector3 b)
{
    vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float32 vector3_norm_squared(vector3 a)
{
    float result = vector3_dot(a, a);
    return result;
}

float32 vector3_norm(vector3 a)
{
    float result = sqrtf(vector3_norm_squared(a));
    return result;
}

vector3 vector3_normalize(vector3 a)
{
    float norm = vector3_norm(a);
    if (norm > 0)
    {
        a.x = a.x / norm;
        a.y = a.y / norm;
        a.z = a.z / norm;
    }
    return a;
}

// ============ vector3i ============ //

vector3i vector3i_create(int x, int y, int z)
{
    vector3i result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

// ============ vector4 ============ //

vector4 vector4_create(float x, float y, float z, float w)
{
    vector4 result = { .x = x, .y = y, .z = z, .w = w };
    return result;
}

vector4 vector4_negate(vector4 v)
{
    vector4 result = { .x = -v.x, .y = -v.y, .z = -v.z, .w = -v.w };
    return result;
}

vector4 vector4_scale(float32 a, vector4 v)
{
    vector4 result = { .x = a * v.x, .y = a * v.y, .z = a * v.z, .w = a * v.w };
    return result;
}

vector4 vector4_add(vector4 a, vector4 b)
{
    vector4 result = { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
    return result;
}

vector4 vector4_sub(vector4 a, vector4 b)
{
    vector4 result = { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
    return result;
}

float32 vector4_dot(vector4 a, vector4 b)
{
    float32 result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    return result;
}

float32 vector4_norm_squared(vector4 v)
{
    float32 result = vector4_dot(v, v);
    return result;
}

float32 vector4_norm(vector4 v)
{
    float32 result = sqrtf(vector4_norm_squared(v));
    return result;
}

vector4 vector4_normalize(vector4 v)
{
    float32 norm = vector4_norm(v);
    if (norm > 0)
    {
        v.x = v.x / norm;
        v.y = v.y / norm;
        v.z = v.z / norm;
        v.w = v.w / norm;
    }
    return v;
}

// ============ quaternion ============ //

quaternion quaternion_create(float w, float x, float y, float z)
{
    quaternion result = {};
    result.w = w;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

quaternion quaternion_create_identity(void)
{
    quaternion result = {};
    result.r = 1.0f;
    return result;
}

quaternion quaternion_create_pure(vector3 v)
{
    quaternion result = {};
    result.axis = v;
    return result;
}

quaternion quaternion_rotate_x(float rx)
{
    quaternion result = {};
    result._1 = cosf(0.5f * rx);
    result._e32 = sinf(0.5f * rx);
    return result;
}

quaternion quaternion_rotate_y(float ry)
{
    quaternion result = {};
    result._1 = cosf(0.5f * ry);
    result._e13 = sinf(0.5f * ry);
    return result;
}

quaternion quaternion_rotate_z(float rz)
{
    quaternion result = {};
    result._1 = cosf(0.5f * rz);
    result._e21 = sinf(0.5f * rz);
    return result;
}

quaternion quaternion_rotate(float radians, vector3 axis)
{
    quaternion result = {};
    result._1 = cosf(0.5f * radians);
    result.axis = vector3_scale(sinf(0.5f * radians), axis);
    return result;
}

quaternion quaternion_scale(float a, quaternion q)
{
    quaternion result;
    result._1 = a * q._1;
    result._e21 = a * q._e21;
    result._e32 = a * q._e32;
    result._e13 = a * q._e13;
    return result;
}

quaternion quaternion_add(quaternion a, quaternion b)
{
    quaternion result;
    result._1 = a._1 + b._1;
    result._e21 = a._e21 + b._e21;
    result._e32 = a._e32 + b._e32;
    result._e13 = a._e13 + b._e13;
    return result;
}

quaternion quaternion_sub(quaternion a, quaternion b)
{
    quaternion result;
    result._1 = a._1 - b._1;
    result._e21 = a._e21 - b._e21;
    result._e32 = a._e32 - b._e32;
    result._e13 = a._e13 - b._e13;
    return result;
}

quaternion quaternion_mul(quaternion a, quaternion b)
{
    quaternion r;
    r._1   = a._1*b._1   - a._e32*b._e32 - a._e13*b._e13 - a._e21*b._e21;
    r._e32 = a._1*b._e32 - a._e21*b._e13 + a._e32*b._1   + a._e13*b._e21;
    r._e13 = a._1*b._e13 + a._e21*b._e32 - a._e32*b._e21 + a._e13*b._1;
    r._e21 = a._1*b._e21 + a._e21*b._1   + a._e32*b._e13 - a._e13*b._e32;
    return r;
}

quaternion quaternion_conjugate(quaternion q)
{
    quaternion result;
    result._1 = q._1;
    result._e32 = -q._e32;
    result._e13 = -q._e13;
    result._e21 = -q._e21;
    return result;
}

float32 quaternion_norm_squared(quaternion q)
{
    float32 result = q._1*q._1 + q._e32*q._e32 + q._e13*q._e13 + q._e21*q._e21;
    return result;
}

float32 quaternion_norm(quaternion q)
{
    float32 result = sqrtf(quaternion_norm_squared(q));
    return result;
}

quaternion quaternion_normalize(quaternion q)
{
    float32 n = quaternion_norm(q);
    if (n > 0)
    {
        q._1 = q._1 / n;
        q._e32 = q._e32 / n;
        q._e21 = q._e21 / n;
        q._e13 = q._e13 / n;
    }
    return q;
}

quaternion quaternion_inverse(quaternion q)
{
    float n2 =  quaternion_norm_squared(q);
    if (n2 > 0)
    {
        q = quaternion_scale(1.f / n2, quaternion_conjugate(q));
    }
    return q;
}

vector3 quaternion_apply_unit(quaternion q, vector3 v)
{
    float32 ww = square(q.w);
    float32 xx = square(q.x);
    float32 yy = square(q.y);
    float32 zz = square(q.z);

    vector3 result;
    result.x = (ww + xx - yy - zz)*v.x + 2.f*(( q.y*v.z - q.z*v.y)*q.w + (q.y*v.y + q.z*v.z)*q.x);
    result.y = (ww - xx + yy - zz)*v.y + 2.f*((-q.x*v.z + q.z*v.x)*q.w + (q.x*v.x + q.z*v.z)*q.y);
    result.z = (ww - xx - yy + zz)*v.z + 2.f*(( q.x*v.y - q.y*v.x)*q.w + (q.x*v.x + q.y*v.y)*q.z);
    return result;
}

vector3 quaternion_apply(quaternion q, vector3 v)
{
    q = quaternion_normalize(q);
    vector3 result = quaternion_apply_unit(q, v);
    return result;
}

// ============ matrix3 ============ //

matrix3 matrix3_identity(void)
{
    matrix3 result = {};
    result._11 = result._22 = result._33 = 1.f;
    return result;
}

matrix3 matrix3_scale(float a, matrix3 m)
{
    int i, j;
    matrix3 r;
    for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
        r.e[i][j] = a * m.e[i][j];
    return r;
}

float32 matrix3_determinant(matrix3 m)
{
    float32 result = m._11 * (m._22 * m._33 - m._23 * m._32)
                   - m._12 * (m._21 * m._33 - m._23 * m._31)
                   + m._13 * (m._21 * m._32 - m._22 * m._31);
    return result;
}

matrix3 matrix3_adjoint(matrix3 m)
{
    matrix3 result;

    result._11 =  m._22 * m._33 - m._23 * m._32;
    result._12 = -m._12 * m._33 + m._13 * m._32;
    result._13 =  m._12 * m._23 - m._13 * m._22;

    result._21 = -m._21 * m._33 + m._23 * m._31;
    result._22 =  m._11 * m._33 - m._13 * m._31;
    result._23 = -m._11 * m._23 + m._13 * m._21;

    result._31 =  m._21 * m._32 - m._22 * m._31;
    result._32 = -m._11 * m._32 + m._12 * m._31;
    result._33 =  m._11 * m._22 - m._12 * m._21;

    return result;
}

matrix3 matrix3_inverse(matrix3 m)
{
    matrix3 result = matrix3_identity();
    float32 det = matrix3_determinant(m);
    if (!is_near_zero(det))
    {
        result = matrix3_scale(1.0f / det, matrix3_adjoint(m));
    }
    return result;
}

matrix3 quaternion_to_m3f(quaternion q)
{
    float32 s = quaternion_norm(q);
    matrix3 result;

    result._11 = 1.f - 2.f * s * (q.j * q.j + q.k * q.k);
    result._12 =       2.f * s * (q.i * q.j - q.k * q.r);
    result._13 =       2.f * s * (q.i * q.k + q.j * q.r);

    result._21 =       2.f * s * (q.i * q.j + q.k * q.r);
    result._22 = 1.f - 2.f * s * (q.i * q.i + q.k * q.k);
    result._23 =       2.f * s * (q.j * q.k - q.i * q.r);

    result._31 =       2.f * s * (q.i * q.k - q.j * q.r);
    result._32 =       2.f * s * (q.j * q.k + q.i * q.r);
    result._33 = 1.f - 2.f * s * (q.i * q.i + q.j * q.j);

    return result;
}

// ============ matrix4 ============ //

matrix4 matrix4_identity(void)
{
    matrix4 result = {};
    result._11 = 1.f;
    result._22 = 1.f;
    result._33 = 1.f;
    result._44 = 1.f;
    return result;
}

matrix4 matrix4_translate(float tx, float ty, float tz)
{
    matrix4 result = matrix4_identity();
    result._14 = tx;
    result._24 = ty;
    result._34 = tz;
    return result;
}

matrix4 matrix4_scale(float sx, float sy, float sz)
{
    matrix4 result = {};
    result._11 = sx;
    result._22 = sy;
    result._33 = sz;
    result._44 = 1.f;
    return result;
}

matrix4 matrix4_mul(matrix4 a, matrix4 b)
{
    matrix4 result;

    result._11 = a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41;
    result._12 = a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42;
    result._13 = a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43;
    result._14 = a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44;

    result._21 = a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41;
    result._22 = a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42;
    result._23 = a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43;
    result._24 = a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44;

    result._31 = a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41;
    result._32 = a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42;
    result._33 = a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43;
    result._34 = a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44;

    result._41 = a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41;
    result._42 = a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42;
    result._43 = a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43;
    result._44 = a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44;

    return result;
}

matrix4 matrix4_transpose(matrix4 m)
{
    float32 tmp;
    tmp = m._12; m._12 = m._21; m._21 = tmp;
    tmp = m._13; m._13 = m._31; m._31 = tmp;
    tmp = m._14; m._14 = m._41; m._41 = tmp;
    tmp = m._23; m._23 = m._32; m._32 = tmp;
    tmp = m._24; m._24 = m._42; m._42 = tmp;
    tmp = m._34; m._34 = m._43; m._43 = tmp;
    return m;
}

matrix4 quaternion_to_m4f(quaternion q)
{
    float32 s = quaternion_norm(q);
    matrix4 result;

    result._11 = 1.f - 2.f * s * (q.j * q.j + q.k * q.k);
    result._12 =       2.f * s * (q.i * q.j - q.k * q.r);
    result._13 =       2.f * s * (q.i * q.k + q.j * q.r);
    result._14 = 0.f;

    result._21 =       2.f * s * (q.i * q.j + q.k * q.r);
    result._22 = 1.f - 2.f * s * (q.i * q.i + q.k * q.k);
    result._23 =       2.f * s * (q.j * q.k - q.i * q.r);
    result._24 = 0.f;

    result._31 =       2.f * s * (q.i * q.k - q.j * q.r);
    result._32 =       2.f * s * (q.j * q.k + q.i * q.r);
    result._33 = 1.f - 2.f * s * (q.i * q.i + q.j * q.j);
    result._34 = 0.f;

    result._41 = 0.f;
    result._42 = 0.f;
    result._43 = 0.f;
    result._44 = 1.f;

    return result;
}

// ============ transform ============ //

transform transform_identity(void) { transform tm = {}; tm._11 = tm._22 = tm._33 = 1.f; return tm; }
transform transform_scale_x (float32 sx) { transform tm = transform_identity(); tm._11 = sx; return tm; }
transform transform_scale_y (float32 sy) { transform tm = transform_identity(); tm._22 = sy; return tm; }
transform transform_scale_z (float32 sz) { transform tm = transform_identity(); tm._33 = sz; return tm; }
transform transform_scale   (float32 sx, float32 sy, float32 sz) { transform tm = transform_identity(); tm._11 = sx; tm._22 = sy; tm._33 = sz; return tm; }
transform transform_translate_x (float32 tx) { transform tm = transform_identity(); tm._41 = tx; return tm; }
transform transform_translate_y (float32 ty) { transform tm = transform_identity(); tm._42 = ty; return tm; }
transform transform_translate_z (float32 tz) { transform tm = transform_identity(); tm._43 = tz; return tm; }
transform transform_translate   (float32 tx, float32 ty, float32 tz) { transform tm = transform_identity(); tm._41 = tx; tm._42 = ty; tm._43 = tz; return tm; }
transform transform_rotate_x (float32 rx) { transform tm = transform_identity(); float c = cosf(rx); float s = sinf(rx); tm._22 = tm._33 = c; tm._23 = -(tm._32 = s); return tm; }
transform transform_rotate_y (float32 ry) { transform tm = transform_identity(); float c = cosf(ry); float s = sinf(ry); tm._11 = tm._33 = c; tm._13 = -(tm._31 = s); return tm; }
transform transform_rotate_z (float32 rz) { transform tm = transform_identity(); float c = cosf(rz); float s = sinf(rz); tm._11 = tm._22 = c; tm._12 = -(tm._21 = s); return tm; }

float32 transform_determinant(transform tm)
{
    float32 result = matrix3_determinant(tm.matrix);
    return result;
}

transform transform_inverse(transform tm)
{
    transform result = {};

    float32 det = matrix3_determinant(tm.matrix);
    if (!is_near_zero(det))
    {
        result.matrix = matrix3_scale(1.0f / det, matrix3_adjoint(tm.matrix));

        float32 m_32_43_33_42 = (tm._32 * tm._43 - tm._33 * tm._42);
        float32 m_31_43_33_41 = (tm._31 * tm._43 - tm._33 * tm._41);
        float32 m_31_42_32_41 = (tm._31 * tm._42 - tm._32 * tm._41);
        float32 m_12_23_13_22 = (tm._12 * tm._23 - tm._13 * tm._22);
        float32 m_11_23_13_21 = (tm._11 * tm._23 - tm._13 * tm._21);
        float32 m_11_22_12_21 = (tm._11 * tm._22 - tm._12 * tm._21);

        result._41 = (- tm._21 * m_32_43_33_42 + tm._22 * m_31_43_33_41 - tm._23 * m_31_42_32_41) / det;
        result._42 = (  tm._11 * m_32_43_33_42 - tm._12 * m_31_43_33_41 + tm._13 * m_31_42_32_41) / det;
        result._43 = (- tm._41 * m_12_23_13_22 + tm._42 * m_11_23_13_21 - tm._43 * m_11_22_12_21) / det;
    }
    return result;
}

matrix4 transform_to_matrix4(transform tm)
{
    matrix4 result;
    result._1.xyz = tm._1; result._1.w = 0;
    result._2.xyz = tm._2; result._2.w = 0;
    result._3.xyz = tm._3; result._3.w = 0;
    result._4.xyz = tm._4; result._4.w = 1;
    result = matrix4_transpose(result);
    return result;
}

vector3 transform_transform_point3f(transform tm, vector3 v)
{
    vector3 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z + tm._41;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z + tm._42;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z + tm._43;
    return result;
}

vector4 transform_transform_point4f(transform tm, vector4 v)
{
    vector4 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z + tm._41*v.w;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z + tm._42*v.w;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z + tm._43*v.w;
    result.w = 1.f;
    return result;
}

vector3 transform_transform_vector3f(transform tm, vector3 v)
{

    vector3 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z;
    return result;
}

vector4 transform_transform_vector4f(transform tm, vector4 v)
{
    vector4 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z;
    result.w = 0.f;
    return result;
}

/*
    F = first
    S = second

    The transform struct is row-major storage for matrix, so
    multiplication performed in left-to-right order.
    We just pretend that operator is (S*F)*vec for consistency
    with the matrix4 interface (which is natural from math class).

    | f11 f12 f13 0 |   | s11 s12 s13 0 |
    | f21 f22 f23 0 | * | s21 s22 s23 0 | =
    | f31 f32 f33 0 |   | s31 s32 s33 0 |
    | f41 f42 f43 1 |   | s41 s42 s43 1 |
*/
transform transform_mul(transform s, transform f)
{
    transform result;

    result._11 = f._11*s._11 + f._12*s._21 + f._13*s._31;
    result._12 = f._11*s._12 + f._12*s._22 + f._13*s._32;
    result._13 = f._11*s._13 + f._12*s._23 + f._13*s._33;

    result._21 = f._21*s._11 + f._22*s._21 + f._23*s._31;
    result._22 = f._21*s._12 + f._22*s._22 + f._23*s._32;
    result._23 = f._21*s._13 + f._22*s._23 + f._23*s._33;

    result._31 = f._31*s._11 + f._32*s._21 + f._33*s._31;
    result._32 = f._31*s._12 + f._32*s._22 + f._33*s._32;
    result._33 = f._31*s._13 + f._32*s._23 + f._33*s._33;

    result._41 = f._41*s._11 + f._42*s._21 + f._43*s._31 + s._41;
    result._42 = f._41*s._12 + f._42*s._22 + f._43*s._32 + s._42;
    result._43 = f._41*s._13 + f._42*s._23 + f._43*s._33 + s._43;

    return result;
}


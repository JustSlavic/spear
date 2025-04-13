#ifndef PHYS_MATRIX
#define PHYS_MATRIX

#include <base.h>


namespace phys {


struct matrix
{
    float64 _11, _12, _13;
    float64 _21, _22, _23;
    float64 _31, _32, _33;

    FORCE_INLINE static matrix identity() { matrix m = {}; m._11 = m._22 = m._33 = 1.f; return m; }
};

FORCE_INLINE matrix & operator += (matrix & a, matrix b) { a._11 += b._11; a._12 += b._12; a._13 += b._13; a._21 += b._21; a._22 += b._22; a._23 += b._23; a._31 += b._31; a._32 += b._32; a._33 += b._33; return a; }
FORCE_INLINE matrix & operator -= (matrix & a, matrix b) { a._11 -= b._11; a._12 -= b._12; a._13 -= b._13; a._21 -= b._21; a._22 -= b._22; a._23 -= b._23; a._31 -= b._31; a._32 -= b._32; a._33 -= b._33; return a; }
FORCE_INLINE matrix & operator *= (matrix & a, float64 c) { a._11 *= c;     a._12 *= c;     a._13 *= c;     a._21 *= c;     a._22 *= c;     a._23 *= c;     a._31 *= c;     a._32 *= c;     a._33 *= c;     return a; }
FORCE_INLINE matrix & operator /= (matrix & a, float64 c) { a._11 /= c;     a._12 /= c;     a._13 /= c;     a._21 /= c;     a._22 /= c;     a._23 /= c;     a._31 /= c;     a._32 /= c;     a._33 /= c;     return a; }

FORCE_INLINE matrix operator - (matrix a) { matrix r; r._11 = -a._11; r._12 = -a._12; r._13 = -a._13; r._21 = -a._21; r._22 = -a._22; r._23 = -a._23; r._31 = -a._31; r._32 = -a._32; r._33 = -a._33; return r; }
FORCE_INLINE matrix operator + (matrix a, matrix b) { matrix r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._13 = a._13 + b._13; r._21 = a._21 + b._21; r._22 = a._22 + b._22; r._23 = a._23 + b._23; r._31 = a._31 + b._31; r._32 = a._32 + b._32; r._33 = a._33 + b._33; return r; }
FORCE_INLINE matrix operator - (matrix a, matrix b) { matrix r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._13 = a._13 - b._13; r._21 = a._21 - b._21; r._22 = a._22 - b._22; r._23 = a._23 - b._23; r._31 = a._31 - b._31; r._32 = a._32 - b._32; r._33 = a._33 - b._33; return r; }
FORCE_INLINE matrix operator * (matrix a, float64 c) { matrix r; r._11 = a._11 * c; r._12 = a._12 * c; r._13 = a._13 * c; r._21 = a._21 * c; r._22 = a._22 * c; r._23 = a._23 * c; r._31 = a._31 * c; r._32 = a._32 * c; r._33 = a._33 * c; return r; }
FORCE_INLINE matrix operator * (float64 c, matrix a) { matrix r; r._11 = c * a._11; r._12 = c * a._12; r._13 = c * a._13; r._21 = c * a._21; r._22 = c * a._22; r._23 = c * a._23; r._31 = c * a._31; r._32 = c * a._32; r._33 = c * a._33; return r; }
FORCE_INLINE matrix operator / (matrix a, float64 c) { matrix r; r._11 = a._11 / c; r._12 = a._12 / c; r._13 = a._13 / c; r._21 = a._21 / c; r._22 = a._22 / c; r._23 = a._23 / c; r._31 = a._31 / c; r._32 = a._32 / c; r._33 = a._33 / c; return r; }

FORCE_INLINE vector operator * (matrix a, vector v)
{
    vector result;

    result.x = a._11*v._e1 + a._12*v._e2 + a._13*v._e3;
    result.y = a._21*v._e1 + a._22*v._e2 + a._23*v._e3;
    result.z = a._31*v._e1 + a._32*v._e2 + a._33*v._e3;

    return result;
}

FORCE_INLINE vector operator * (vector v, matrix a)
{
    vector result;

    result.x = v._e1*a._11 + v._e2*a._21 + v._e3*a._31;
    result.y = v._e1*a._12 + v._e2*a._22 + v._e3*a._32;
    result.z = v._e1*a._13 + v._e2*a._23 + v._e3*a._33;

    return result;
}

FORCE_INLINE matrix operator * (matrix a, matrix b)
{
    matrix result;

    result._11 = a._11*b._11 + a._12*b._21 + a._13*b._31;
    result._12 = a._11*b._12 + a._12*b._22 + a._13*b._32;
    result._13 = a._11*b._13 + a._12*b._23 + a._13*b._33;

    result._21 = a._21*b._11 + a._22*b._21 + a._23*b._31;
    result._22 = a._21*b._12 + a._22*b._22 + a._23*b._32;
    result._23 = a._21*b._13 + a._22*b._23 + a._23*b._33;

    result._31 = a._31*b._11 + a._32*b._21 + a._33*b._31;
    result._32 = a._31*b._12 + a._32*b._22 + a._33*b._32;
    result._33 = a._31*b._13 + a._32*b._23 + a._33*b._33;

    return result;
}

FORCE_INLINE void transpose(matrix & m)
{
    float64 tmp;
    tmp = m._12; m._12 = m._21; m._21 = tmp;
    tmp = m._13; m._13 = m._31; m._31 = tmp;
    tmp = m._23; m._23 = m._32; m._32 = tmp;
}

FORCE_INLINE matrix transposed(matrix m)
{
    transpose(m);
    return m;
}

FORCE_INLINE float64 determinant(matrix const & m)
{
    float64 result = m._11 * (m._22 * m._33 - m._23 * m._32)
                   - m._12 * (m._21 * m._33 - m._23 * m._31)
                   + m._13 * (m._21 * m._32 - m._22 * m._31);
    return result;
}

FORCE_INLINE matrix to_matrix(matrix3 const & m)
{
    matrix result;
    result._11 = m._11; result._12 = m._12; result._13 = m._13;
    result._21 = m._21; result._22 = m._22; result._23 = m._23;
    result._31 = m._31; result._32 = m._32; result._33 = m._33;
    return result;
}

FORCE_INLINE matrix3 to_matrix3(matrix const & m)
{
    matrix3 result;
    result._11 = m._11; result._12 = m._12; result._13 = m._13;
    result._21 = m._21; result._22 = m._22; result._23 = m._23;
    result._31 = m._31; result._32 = m._32; result._33 = m._33;
    return result;
}

} // namespace phys

#endif // PHYS_MATRIX

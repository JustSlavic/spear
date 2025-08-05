#ifndef _SPEAR_CORELIBS_BASE_VECTOR2_H
#define _SPEAR_CORELIBS_BASE_VECTOR2_H

#include "base.h"


typedef union vector2f
{
    struct { float32 x, y; };
    struct { float32 _e1, _e2; };
    float32 e[2];
} vector2f;
typedef vector2f vector2;

vector2 v2f(float x, float y);
vector2 v2f_negate(vector2 a);
vector2 v2f_scale(float32 a, vector2 v);
vector2 v2f_add(vector2 a, vector2 b);
vector2 v2f_sub(vector2 a, vector2 b);
float32 v2f_dot(vector2 a, vector2 b);
float32 v2f_norm_squared(vector2 a);
float32 v2f_norm(vector2 a);
vector2 v2f_normalize(vector2 a);

typedef union vector2i
{
    struct { int32 x, y; };
    int32 e[2];
} vector2i;

typedef union vector3f
{
    struct { float32 x, y, z; };
    struct { float32 r, g, b; };
    struct { vector2 xy; };
    struct { float32 _e1, _e2, _e3; };
    float32 e[3];
} vector3f;
typedef vector3f vector3;

vector3 v3f(float x, float y, float z);
vector3 v3f_negate(vector3 a);
vector3 v3f_scale(float32 a, vector3 v);
vector3 v3f_add(vector3 a, vector3 b);
vector3 v3f_sub(vector3 a, vector3 b);
float32 v3f_dot(vector3 a, vector3 b);
vector3 v3f_cross(vector3 a, vector3 b);
float32 v3f_norm_squared(vector3 a);
float32 v3f_norm(vector3 a);
vector3 v3f_normalize(vector3 a);

typedef union vector3i
{
    struct { int32 x, y, z; };
    int32 e[3];
} vector3i;

vector3i v3i(int x, int y, int z);

typedef union vector4f
{
    struct { float32 x, y, z, w; };
    struct { float32 r, g, b, a; };
    struct { vector3 xyz; };
    struct { vector3 rgb; };
    struct { float32 _e1, _e2, _e3, _e4; };
    float32 e[4];
} vector4f;
typedef vector4f vector4;

vector4 v4f(float x, float y, float z, float w);
vector4 v4f_negate(vector4 v);
vector4 v4f_scale(float32 a, vector4 v);
vector4 v4f_add(vector4 v, vector4 w);
vector4 v4f_sub(vector4 v, vector4 w);
float32 v4f_dot(vector4 v, vector4 w);
vector4 v4f_cross(vector4 v, vector4 w);
float32 v4f_norm_squared(vector4 v);
float32 v4f_norm(vector4 v);
vector4 v4f_normalize(vector4 v);

typedef union vector4i
{
    struct { int32 x, y, z, w; };
    struct { int32 _e1, _e2, _e3, _e4; };
    int32 e[4];
} vector4i;

typedef union complex
{
    struct { float32 re, im; };
    struct { float32 _1, _e12; };
    float32 e[2];
} complex;

typedef union quaternion
{
    /*
        Hamilton multiplication table
              |  i  j  k
            --+--------
            i | -1  k -j
            j | -k -1  i
            k |  j -i -1
    */
    struct { float32 w, x, y, z; };
    struct { float32 r, i, j, k; };
    struct { float32 _1, _e32, _e13, _e21; };
    struct { float32 pad1; vector3 axis; };
    float32 e[4];
} quaternion;

quaternion q4f(float w, float x, float y, float z);
quaternion q4f_identity(void);
quaternion q4f_rotate_x(float rx);
quaternion q4f_rotate_y(float ry);
quaternion q4f_rotate_z(float rz);
quaternion q4f_rotate(float radians, vector3 axis);
quaternion q4f_pure(vector3 v);
quaternion q4f_scale(float a, quaternion q);
quaternion q4f_add(quaternion a, quaternion b);
quaternion q4f_sub(quaternion a, quaternion b);
quaternion q4f_mul(quaternion a, quaternion b);
quaternion q4f_conjugate(quaternion a);
float32 q4f_norm_squared(quaternion a);
float32 q4f_norm(quaternion a);
quaternion q4f_normalize(quaternion a);
quaternion q4f_inverse(quaternion a);
vector3 q4f_apply_unit_quaternion(quaternion q, vector3 v);
vector3 q4f_apply(quaternion q, vector3 v);

typedef union matrix2
{
    struct
    {
        float32 _11, _12;
        float32 _21, _22;
    };
    vector2 row[2];
    float32 e[2][2];
} matrix2;

typedef union matrix3
{
    struct
    {
        float32 _11, _12, _13;
        float32 _21, _22, _23;
        float32 _31, _32, _33;
    };
    struct
    {
        vector3 _1, _2, _3;
    };
    vector3 row[3];
    float32 e[3][3];
} matrix3;

typedef union matrix4
{
    struct
    {
        float32 _11, _12, _13, _14;
        float32 _21, _22, _23, _24;
        float32 _31, _32, _33, _34;
        float32 _41, _42, _43, _44;
    };
    struct
    {
        vector4 _1, _2, _3, _4;
    };
    vector4 row[4];
    float32 e[4][4];
} matrix4;

typedef union transform
{
    struct
    {
        float32 _11, _12, _13;
        float32 _21, _22, _23;
        float32 _31, _32, _33;
        float32 _41, _42, _43;
    };
    struct { matrix3 matrix; vector3 displacement; };
    struct { vector3 sx, sy, sz, sd; };
    struct { vector3 _1, _2, _3, _4; };
    float32 e[4][3];
} transform;

matrix4 identitym4f(void);
matrix4 translatem4f(float tx, float ty, float tz);
matrix4 scalem4f(float sx, float sy, float sz);

matrix4 mulm4f(matrix4 a, matrix4 b);


#endif // _SPEAR_CORELIBS_BASE_VECTOR2_H

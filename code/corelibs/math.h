#ifndef _SPEAR_CORELIBS_BASE_VECTOR2_H
#define _SPEAR_CORELIBS_BASE_VECTOR2_H

#include "base.h"


typedef struct { float32 x, y;       } vector2;

typedef union
{
    struct { float32 x, y, z; };
    float32 e[3];
} vector3;

typedef union
{
    struct { float32 x, y, z, w; };
    struct { float32 r, g, b, a; };
    struct { vector3 xyz; float32 pad1; };
    struct { vector3 rgb; float32 pad2; };
    float32 e[4];
} vector4;

typedef struct { int32 x, y;       } vector2i;
typedef struct { int32 x, y, z;    } vector3i;
typedef struct { int32 x, y, z, w; } vector4i;

typedef struct { float32 re, im; } complex;

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


vector2 vector2_create(float x, float y);
vector2 vector2_negate(vector2 a);
vector2 vector2_scale(float32 a, vector2 v);
vector2 vector2_add(vector2 a, vector2 b);
vector2 vector2_sub(vector2 a, vector2 b);
float32 vector2_dot(vector2 a, vector2 b);
float32 vector2_norm_squared(vector2 a);
float32 vector2_norm(vector2 a);
vector2 vector2_normalize(vector2 a);

vector3 vector3_create(float x, float y, float z);
vector3 vector3_negate(vector3 a);
vector3 vector3_scale(float32 a, vector3 v);
vector3 vector3_add(vector3 a, vector3 b);
vector3 vector3_sub(vector3 a, vector3 b);
float32 vector3_dot(vector3 a, vector3 b);
vector3 vector3_cross(vector3 a, vector3 b);
float32 vector3_norm_squared(vector3 a);
float32 vector3_norm(vector3 a);
vector3 vector3_normalize(vector3 a);

vector3i vector3i_create(int x, int y, int z);

vector4 vector4_create(float x, float y, float z, float w);
vector4 vector4_negate(vector4 v);
vector4 vector4_scale(float32 a, vector4 v);
vector4 vector4_add(vector4 v, vector4 w);
vector4 vector4_sub(vector4 v, vector4 w);
float32 vector4_dot(vector4 v, vector4 w);
float32 vector4_norm_squared(vector4 v);
float32 vector4_norm(vector4 v);
vector4 vector4_normalize(vector4 v);

quaternion quaternion_create(float w, float x, float y, float z);
quaternion quaternion_create_pure(vector3 v);
quaternion quaternion_create_identity(void);
quaternion quaternion_rotate_x(float rx);
quaternion quaternion_rotate_y(float ry);
quaternion quaternion_rotate_z(float rz);
quaternion quaternion_rotate(float radians, vector3 axis);
quaternion quaternion_scale(float a, quaternion q);
quaternion quaternion_add(quaternion a, quaternion b);
quaternion quaternion_sub(quaternion a, quaternion b);
quaternion quaternion_mul(quaternion a, quaternion b);
quaternion quaternion_conjugate(quaternion a);
float32 quaternion_norm_squared(quaternion a);
float32 quaternion_norm(quaternion a);
quaternion quaternion_normalize(quaternion a);
quaternion quaternion_inverse(quaternion a);
vector3 quaternion_apply_unit(quaternion q, vector3 v);
vector3 quaternion_apply(quaternion q, vector3 v);

matrix3 matrix3_identity(void);
matrix3 matrix3_scale(float a, matrix3 m);
float32 matrix3_determinant(matrix3 m);
matrix3 matrix3_adjoint(matrix3 m);
matrix3 matrix3_inverse(matrix3 m);
matrix3 quaternion_to_matrix3(quaternion q);

matrix4 matrix4_identity(void);
matrix4 matrix4_translate(float tx, float ty, float tz);
matrix4 matrix4_scale(float sx, float sy, float sz);
matrix4 matrix4_mul(matrix4 a, matrix4 b);
matrix4 quaterion_to_matrix4(quaternion q);

transform transform_identity(void);
transform transform_scale_x(float sx);
transform transform_scale_y(float sy);
transform transform_scale_z(float sz);
transform transform_scale(float sx, float sy, float sz);
transform transform_translate_x(float tx);
transform transform_translate_y(float ty);
transform transform_translate_z(float tz);
transform transform_translate(float tx, float ty, float tz);
transform transform_rotate_x(float rx);
transform transform_rotate_y(float ry);
transform transform_rotate_z(float rz);
float transform_determinant(transform tm);
transform transform_inverse(transform tm);
matrix4 transform_to_matrix4(transform tm);
vector3 transform_transform_point3f(transform tm, vector3 v);
vector4 transform_transform_point4f(transform tm, vector4 v);
vector3 transform_transform_vector3f(transform tm, vector3 v);
vector4 transform_transform_vector4f(transform tm, vector4 v);
transform transform_mul(transform s, transform f);


#endif // _SPEAR_CORELIBS_BASE_VECTOR2_H

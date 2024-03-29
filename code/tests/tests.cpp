#include <base.h>

#include <stdio.h>
#include <stdlib.h>

#include <math/matrix2.hpp>
#include <math/matrix3.hpp>
#include <math/matrix4.hpp>

#include <math/predicates.hpp>

#include <math/complex.hpp>
#include <math/quaternion.hpp>
#include <math/unitary2.hpp>
#include <math/transform.hpp>
#include <math/clifford2.hpp>
#include <math/clifford3.hpp>

// @todo:
// - color the failed and passed words in terminal

#define TEST_BEGIN(...) \
    int32 passed_tests_count = 0; \
    int32 failed_tests_count = 0; \
    (void)0

#define TEST_END(...) \
    printf("--------------------\n"); \
    printf("Tests passed: %d\n", passed_tests_count); \
    printf("Tests failed: %d\n", failed_tests_count); \
    (void)0

#define TEST(NAME) \
    static bool32 already_printed_result_##NAME = false; \
    void test_##NAME(bool32 *success)

#define TEST_PRINT_OK_RESULT(NAME) \
    if (!already_printed_result_##NAME) { \
        printf("Ok.\n"); \
        already_printed_result_##NAME = true; \
    } (void)0

#define TEST_RUN(NAME) \
    do { \
        printf("TEST %s... ", STRINGIFY(NAME)); \
        bool32 success = true; \
        test_##NAME(&success); \
        if (success) { \
            passed_tests_count += 1; \
            TEST_PRINT_OK_RESULT(NAME); \
        } else { \
            failed_tests_count += 1; \
        } \
    } while(0)

#define TEST_ASSERT(COND) \
    do { \
        if (COND) { } else { \
            printf("\n%s:%d FAIL\n", __FILE__, __LINE__); \
            *success = false; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_EQ(A, B) \
    do { \
        if (A == B) { } else { \
            printf("\n%s:%d FAIL (%s != %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_NEQ(A, B) \
    do { \
        if (A != B) { } else { \
            printf("\n%s:%d FAIL (%s == %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_EQ(A, B) \
    do { \
        if (::math::equal(A, B)) { } else { \
            printf("\n%s:%d FAIL (%s != %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while (0)

#define TEST_ASSERT_FLOAT_NEQ(A, B) \
    do { \
        if (!::math::equal(A, B)) { } else { \
            printf("\n%s:%d FAIL (%s == %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while (0)


// ----------------------------------------------


TEST(Matrix2)
{
    // Determinant
    {
        auto m1 = math::make_matrix2(1, 0,
                                     0, 1);
        auto det1 = determinant(m1);
        TEST_ASSERT_FLOAT_EQ(det1, 1.f);

        auto m2 = math::make_matrix2(2, 0,
                                     0, 2);
        auto det2 = determinant(m2);
        TEST_ASSERT_FLOAT_EQ(det2, 4.f);

        auto m3 = math::make_matrix2(1, 0,
                                     9, 1);
        auto det3 = determinant(m3);
        TEST_ASSERT_FLOAT_EQ(det3, 1.f);
    }
    // Inverse
    {
        auto m = math::make_matrix2(1, 2,
                                    100, 1);

        TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

        auto inversed = inverse(m);
        auto id = m * inversed;

        TEST_ASSERT_FLOAT_EQ(id, math::matrix2::identity());
    }
}

TEST(Matrix3)
{
    // Determinant
    {
        auto m1 = math::make_matrix3(1, 0, 0,
                                     0, 1, 0,
                                     0, 0, 1);
        auto det1 = determinant(m1);
        TEST_ASSERT_FLOAT_EQ(det1, 1.f);

        auto m2 = math::make_matrix3(2, 0, 0,
                                     0, 2, 0,
                                     0, 0, 2);
        auto det2 = determinant(m2);
        TEST_ASSERT_FLOAT_EQ(det2, 8.f);

        auto m3 = math::make_matrix3(1, 0, 0,
                                     9, 2, 0,
                                     9, 9, 2);
        auto det3 = determinant(m3);
        TEST_ASSERT_FLOAT_EQ(det3, 4.f);

        auto m4 = math::make_matrix3(2, 5, 3,
                                     6, 1, 4,
                                     7, 8, 9);
        auto det4 = determinant(m4);
        TEST_ASSERT_FLOAT_EQ(det4, -53.f);
    }
    // Inverse
    {
        auto m = math::make_matrix3(2, 5, 9,
                                    6, 1, 4,
                                    7, 8, 3);
        TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

        auto inversed = inverse(m);
        auto id = m * inversed;
        TEST_ASSERT_FLOAT_EQ(id, math::matrix3::identity());
    }
}

TEST(Matrix4)
{
    // Determinant
    {
        auto m = math::make_matrix4(1, 1, 1, 7,
                                    1, 5, 9, 1,
                                    0, 4, 5, 5,
                                    9, 1, 1, 2);
        TEST_ASSERT_FLOAT_EQ(determinant(m), 524.f);
    }
    // Inverse
    {
        auto m = math::make_matrix4(1, 1, 1, 7,
                                    1, 5, 9, 1,
                                    0, 4, 5, 5,
                                    9, 1, 1, 2);
        TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

        auto inversed = inverse(m);
        auto id = m * inversed;
        TEST_ASSERT_FLOAT_EQ(id, math::matrix4::identity());
    }
}

TEST(Transform)
{
    auto m = math::make_matrix4(1,   2,  1, 0,
                                3,   2,  2, 0,
                                1,   1,  2, 0,
                                10, 10, -2, 1);
    TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

    auto tm = math::make_transform(1, 2, 1,
                                   3, 2, 2,
                                   1, 1, 2,
                                   10, 10, -2);
    // The transform does exactly as the matrix4
    {
        auto v = V4(4, 1, -8, 1);

        auto w = v * m;
        auto u = tm * v.xyz;

        TEST_ASSERT_FLOAT_EQ(w.x, u.x);
        TEST_ASSERT_FLOAT_EQ(w.y, u.y);
        TEST_ASSERT_FLOAT_EQ(w.z, u.z);
        TEST_ASSERT_FLOAT_EQ(w.w, 1.f);
    }
    // Inverse is correct
    {
        auto inv_m = inverse(m);
        auto inv_tm = inverse(tm);

        UNUSED(inv_m);
        UNUSED(inv_tm);

        auto v = V4(4, 1, -8, 1);

        auto w = v * inv_m;
        auto u = inv_tm * v.xyz;

        TEST_ASSERT_FLOAT_EQ(w.x, u.x);
        TEST_ASSERT_FLOAT_EQ(w.y, u.y);
        TEST_ASSERT_FLOAT_EQ(w.z, u.z);
        TEST_ASSERT_FLOAT_EQ(w.w, 1.f);
    }
}

TEST(ComplexNumbers)
{
    {
        auto a = -1_i;
        auto b = 1 + 1_i;

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re,  1.f);
        TEST_ASSERT_FLOAT_EQ(c.im, -1.f);
    }
    {
        auto a = 5 + 2_i;
        auto b = 3 + 0.5_i;

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re, 14.f);
        TEST_ASSERT_FLOAT_EQ(c.im, 8.5f);
    }
    {
        auto a = 1.f;
        auto b = 1_i;

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re, 0.f);
        TEST_ASSERT_FLOAT_EQ(c.im, 1.f);
    }
}

TEST(Quaternions)
{
    // Right hand rule:
    // Ox -> to the right
    // Oy  ^ to up
    // Oz  . to your face
    // Then rotation around Oy counter clockwise (as it is the natural rotation)
    // V3(1, 0, 0) => V3(0, 0, -1)
    {
        auto q = Q(V3(0, 1, 0), 90_degrees);
        auto v = V3(1, 0, 0);

        auto w = math::rotate_by_unit_quaternion(q, v);

        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }
    {
        auto q = Q(V3(0, 1, 0), 90_degrees);
        auto v = V3(1, 0, 0);

        auto w = math::rotate_by_unit_quaternion2(q, v);

        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }

    // Pure unit quaternions are representing a rotation 180_degrees around their respective axis
    {
        auto q = Q(1, 0, 0, 0);
        auto v = V3(1, 0, 0);

        auto w = math::rotate_by_unit_quaternion(q, v);
        TEST_ASSERT_FLOAT_EQ(w.x, 1.f);
        TEST_ASSERT_FLOAT_EQ(w.y, 0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, 0.f);

        v = V3(0, 1, 0);
        w = math::rotate_by_unit_quaternion(q, v);
        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y, -1.f);
        TEST_ASSERT_FLOAT_EQ(w.z,  0.f);

        v = V3(0, 0, 1);
        w = math::rotate_by_unit_quaternion(q, v);
        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }
}

TEST(SpecialUnitaryMatrices2)
{
    // Test that sigma_x * sigma_x is identity matrix
    {
        auto sigma_x = math::unitary2::sigma_x();
        auto squared = sigma_x * sigma_x;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(1, 0));
    }
    // Test that sigma_y * sigma_y is identity matrix
    {
        auto sigma_y = math::unitary2::sigma_y();
        auto squared = sigma_y * sigma_y;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(1, 0));
    }
    // Test that sigma_z * sigma_z is identity matrix
    {
        auto sigma_z = math::unitary2::sigma_z();
        auto squared = sigma_z * sigma_z;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(1, 0));
    }
    // Test that (sigma_x * sigma_y) squared is negatie identity
    {
        auto sigma_x = math::unitary2::sigma_x();
        auto sigma_y = math::unitary2::sigma_y();
        auto sigma_xy = sigma_x * sigma_y;
        auto squared = sigma_xy * sigma_xy;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(-1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(-1, 0));
    }
    // Test that (sigma_y * sigma_z) squared is negatie identity
    {
        auto sigma_y = math::unitary2::sigma_y();
        auto sigma_z = math::unitary2::sigma_z();
        auto sigma_yz = sigma_y * sigma_z;
        auto squared = sigma_yz * sigma_yz;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(-1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(-1, 0));
    }
    // Test that (sigma_z * sigma_x) squared is negatie identity
    {
        auto sigma_z = math::unitary2::sigma_z();
        auto sigma_x = math::unitary2::sigma_x();
        auto sigma_zx = sigma_z * sigma_x;
        auto squared = sigma_zx * sigma_zx;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(-1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(-1, 0));
    }
    // Right hand rule:
    // Ox -> to the right
    // Oy  ^ to up
    // Oz  . to your face
    // Then rotation around Oy counter-clockwise (as it is the natural rotation)
    // means rotating a vector in the XZ plane, which is denoted by `sigma_z * sigma_x`
    // V3(1, 0, 0) => V3(0, 0, -1)
    {
        auto plane_of_rotation = math::unitary2::sigma_z() * math::unitary2::sigma_x();
        auto v = V3(1, 0, 0);
        auto w = math::rotate_by_unitary2(plane_of_rotation, math::to_radians(90), v);

        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }
}


#if 0


struct game_state {};

TEST(Ui)
{
    usize memory_size = MEGABYTES(1);
    void *memory = malloc(memory_size);
    TEST_ASSERT(memory);

    game_state gs = {};
    auto ui_system = ui::system{};
    ui::initialize(&gs, &ui_system, memory_block{memory, memory_size});

    auto group_1 = ui::make_group(&ui_system, &ui_system.root);
    group_1->position.xy = V2(100, 100);
    auto shape_1 = ui::make_shape(&ui_system, group_1);
    shape_1->position.xy = V2(100, 100);

    auto group_2 = ui::make_group(&ui_system, &ui_system.root);
    auto shape_2 = ui::make_shape(&ui_system, group_2);

    // @note: This should be applied each frame after update phase, right?
    update_transforms(&ui_system);

    TEST_ASSERT(group_1);
    TEST_ASSERT(shape_1);

    TEST_ASSERT(group_2);
    TEST_ASSERT(shape_2);

    TEST_ASSERT_FLOAT_EQ(shape_1->transform.e[3][0], 100.f);
    TEST_ASSERT_FLOAT_EQ(shape_1->transform.e[3][1], 100.f);

    math::vector3 root_of_shape_1 = shape_1->transform * V3(0);
    math::vector3 root_of_shape_1_in_screen_coords = shape_1->transform_to_root * V3(0);

    TEST_ASSERT_FLOAT_EQ(root_of_shape_1.x, 100.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1.y, 100.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1.z, 0.f);

    TEST_ASSERT_FLOAT_EQ(root_of_shape_1_in_screen_coords.x, 200.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1_in_screen_coords.y, 200.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1_in_screen_coords.z, 0.f);
}
#endif

TEST(CliffordG2)
{
    using namespace g2;

    // Geometric product e1*e2 = inner(e1, e2) + outer(e1, e2)
    // Because e1 and e2 are orthogonal, inner(e1, e2) = 0
    {
        auto r = e1 * e2;

        TEST_ASSERT_FLOAT_EQ(r._3, 1.f);
    }
    // Complex numbers are represented as Z = X + Y*e1e2 in the G2 algebra,
    // while vectors are represented as V = v1*e1 + v2*e2.
    // Let's check that e1Z is a vector V = X*e1 + Y*e2.
    {
        auto z = 3 + 5*I; // 3 + 0*e1 + 0*e2 + 5*e1e2
        auto v = e1 * z;  // 0 + 3*e1 + 5*e2 + 0*e1e2

        TEST_ASSERT_FLOAT_EQ(v._1, 3.f);
        TEST_ASSERT_FLOAT_EQ(v._2, 5.f);
    }
    // Multiplying complex number by I rotates it by 90 degrees
    {
        auto z = 3 + 5*I;
        auto w = z*I; // (3 + 5e1e2) * e1e2 = 3e1e2 - 5

        TEST_ASSERT_FLOAT_EQ(w.re, -5.f);
        TEST_ASSERT_FLOAT_EQ(w.im, 3.f);
    }
    // Multiplying two complex numbers in Clifford algebra makes
    // the same effect as multiplying them in the complex field
    {
        auto z = 3 + 5*I;
        auto w = 4 + 2*I;
        auto g = z * w;
        // (3 + 5i)(4 + 2i) =
        // = (4*3 - 5*2) + (3*2 + 5*4)i =
        // = 2 + 26i

        TEST_ASSERT_FLOAT_EQ(g.re, 2.0f);
        TEST_ASSERT_FLOAT_EQ(g.im, 26.f);
    }
}

TEST(CliffordG3)
{
    // Test that B1B2B3 == 1
    {
        using namespace g3;

        auto r = XY * YZ * ZX;
        TEST_ASSERT_FLOAT_EQ(r, 1.f);
    }

    // Antisymmetry of orthogonal bivectors
    {
        using namespace g3;

        auto a = YZ * ZX; // e1e3 => -1 * e3e1
        auto b = ZX * YZ; // e3e1 =>  1 * e3e1

        TEST_ASSERT_FLOAT_EQ(a._4, -b._4);
    }

    // Finding the perpendicular plane for any vector can be found by
    // applying a "duality transformation", aS or Sa (it commutes).
    // Let's test it on basis vectors: e1, e2, and e3
    {
        using namespace g3;

        auto plane_e1 = e1 * I;

        // e1e1e2e3 => e2e3

        TEST_ASSERT_FLOAT_EQ(plane_e1._5, 1.f); // e2e3

        auto plane_e2 = e2 * I;

        // e2e1e2e3 => -e1e3 => e3e1

        TEST_ASSERT_FLOAT_EQ(plane_e2._6, 1.f); // e3e1

        auto plane_e3 = e3 * I;

        // e3e1e2e3 => -e1e3e2e3 => e1e2e3e3 => e1e2

        TEST_ASSERT_FLOAT_EQ(plane_e3._4, 1.f); // e1e2
    }

    // The I should also square to -1
    {
        using namespace g3;

        auto sq = I * I;

        TEST_ASSERT_FLOAT_EQ(sq, -1.f);
    }

    // Let's say we have vector A, and vector N,
    // we can get part of the vector A parallel to N,
    // and at the same time the perpendicular to N.
    {
        auto a = g3::make_vector(1, 0, 1);
        auto n = g3::make_vector(1, 0, 0); // @note: n have to be normalized

        auto P = n * inner(n, a); // projection  (n1e1 + n2e2 + n3e3)*(se0)
        auto R = n * outer(n, a); // "rejection" (n1e1 + n2e2 + n3e3)*(r1e12)

        TEST_ASSERT_FLOAT_EQ(P._1, 1.f);
        TEST_ASSERT_FLOAT_EQ(P._2, 0.f);
        TEST_ASSERT_FLOAT_EQ(P._3, 0.f);

        TEST_ASSERT_FLOAT_EQ(R._0, 0.f);
        TEST_ASSERT_FLOAT_EQ(R._1, 0.f);
        TEST_ASSERT_FLOAT_EQ(R._2, 0.f);
        TEST_ASSERT_FLOAT_EQ(R._3, 1.f);
        TEST_ASSERT_FLOAT_EQ(R._4, 0.f);
        TEST_ASSERT_FLOAT_EQ(R._5, 0.f);
        TEST_ASSERT_FLOAT_EQ(R._6, 0.f);
        TEST_ASSERT_FLOAT_EQ(R._7, 0.f);
    }

    // Reflections could be obtained as -nan, where a is the vector,
    // and n is the normal to the reflective plane
    {
        auto n = normalized(g3::make_vector(1, 1, 0));
        auto a = g3::make_vector(23, 4, 5);
        auto b = g3::make_vector(4, 7, 91);

        auto ra = -n*a*n;
        auto rb = -n*b*n;

        TEST_ASSERT_FLOAT_EQ(ra._1, -4.f);
        TEST_ASSERT_FLOAT_EQ(ra._2, -23.f);
        TEST_ASSERT_FLOAT_EQ(ra._3,  5.f);

        TEST_ASSERT_FLOAT_EQ(rb._1, -7.f);
        TEST_ASSERT_FLOAT_EQ(rb._2, -4.f);
        TEST_ASSERT_FLOAT_EQ(rb._3,  91.f);

        // check that angles are the same
        TEST_ASSERT_FLOAT_EQ(inner(a, b), inner(ra, rb));
    }

    // We can reflect bivectors too with the same formula! Except bivectors do not require a minus sign.
    // B' = nBn
    {
        auto b1 = 1.f * g3::e1e2 + 0.f * g3::e2e3 + 0.f * g3::e3e1;
        auto n = g3::make_vector(0, 1, 0); // Length 1 means rotation pi radians

        auto b2 = n*b1*n;

        TEST_ASSERT_FLOAT_EQ(b1._0, -b2._0);
        TEST_ASSERT_FLOAT_EQ(b1._4, -b2._4);
        TEST_ASSERT_FLOAT_EQ(b1._5, -b2._5);
        TEST_ASSERT_FLOAT_EQ(b1._6, -b2._6);
        // printf("b2 = [%f, {%f, %f, %f}, {%f, %f, %f}, %f]\n",
        //     b2._0, b2._1, b2._2, b2._3, b2._4, b2._5, b2._6, b2._7);
    }

    // Rotations are performed by the double-sided transformation
    {
        // Angle between vectors is 90 degrees, so rotation will be in 180 degrees!
        auto n = g3::make_vector(1, 0, 0);
        auto m = g3::make_vector(0, 1, 0);

        auto v = g3::make_vector(2, 2, 0);

        auto rotor = m*n; // In that order!

        auto r = rotor * v * conjugated(rotor);
        auto w = to_vector(r);

        TEST_ASSERT_FLOAT_EQ(v.x, -w.x);
        TEST_ASSERT_FLOAT_EQ(v.y, -w.y);
        TEST_ASSERT_FLOAT_EQ(v.z, -w.z);
    }

    // Let's try to rotate vectors by 90 degrees
    {
        using namespace math;

        // For that we construct two vectors 45 degrees apart in the XY plane
        auto n = g3::make_vector(1, 0, 0);
        auto m = normalized(g3::make_vector(1, 1, 0));

        auto v = g3::make_vector(2, 2, 0);
        //    v = (2, 2)
        // . /
        // ./
        // o. .

        auto rotor = m*n; // In that order!

        auto r = rotor * v * conjugated(rotor);
        auto w = to_vector(r);
        // w = (-2, 2)
        // \ .
        //  \.
        //   o. .

        TEST_ASSERT_FLOAT_EQ(w.x, -2.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  2.f);
        TEST_ASSERT_FLOAT_EQ(w.z,  0.f);
    }
}

// ----------------------------------------------

int32 main(int32 argc, char **argv, char **env)
{
    TEST_BEGIN();

    TEST_RUN(Matrix2);
    TEST_RUN(Matrix3);
    TEST_RUN(Matrix4);
    TEST_RUN(Transform);
    TEST_RUN(ComplexNumbers);
    TEST_RUN(Quaternions);
    TEST_RUN(SpecialUnitaryMatrices2);
    // TEST_RUN(Ui);
    TEST_RUN(CliffordG2);
    TEST_RUN(CliffordG3);

    TEST_END();
    return 0;
}


#include <memory/allocator.cpp>
